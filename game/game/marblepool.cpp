// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#include "marblepool.h"

#include <algorithm>

#include "core/system.h"
#include "core/window.h"

#include "game/collisionmanager.h"
#include "game/gamestate.h"

#include "constants.h"

CollisionObject Marble::getCollisionObject()
{
    return {CollisionType::MarbleObject, &collider, &body};
};

void MarblePool::grow(int count)
{
    size_t current_count = marbles.size();
    size_t final_count = current_count + size_t(count);
    marbles.resize(final_count);
    slots.resize(final_count);
    for(size_t i = final_count; i > current_count; --i) {
        available_slots.push(i-1);
    }
}

size_t MarblePool::availableCount()
{
    return available_slots.size();
}

inline void MarblePool::iterate(std::function<void (int, Marble&)> callback)
{
    auto body_it = marbles.begin();
    auto slot_it = slots.begin();
    int slot_idx = 0;

    while (body_it != marbles.end() && slot_it != slots.end()) {
        if (slot_it->active) {
            callback(slot_idx, *body_it);
        }

        body_it++;
        slot_it++;
        slot_idx++;
    }
}

void MarblePool::iterate(std::function<void (Marble&)> callback)
{
    auto body_it = marbles.begin();
    auto slot_it = slots.begin();

    while (body_it != marbles.end() && slot_it != slots.end()) {
        if (slot_it->active) {
            callback(*body_it);
        }

        body_it++;
        slot_it++;
    }
}

MarblePool::Handle MarblePool::create(Vec2 position, Vec2 velocity, Marble::Type type)
{
    if (availableCount() == 0) grow(64);

    size_t idx = available_slots.top();
    available_slots.pop();

    Handle h;
    h.id = int(idx);
    h.generation = ++slots[idx].generation;
    slots[idx].active = true;

    marbles[idx].collider = Circle(marble_radius);
    marbles[idx].body = PhysicsBody(position, velocity);
    marbles[idx].id = type;

    return h;
}

void MarblePool::release(const Handle& handle)
{
    if (handle.id < 0 || handle.id >= int(slots.size())) {
        return;
    }
    size_t idx = size_t(handle.id);
    Slot s = slots[idx];
    if (s.active && s.generation == handle.generation) {
        slots[idx].active = false;
        available_slots.push(idx);
    }
}

void MarblePool::get(const Handle& handle, std::function<void (Marble&)> callback)
{
    if (handle.id < 0 || handle.id >= int(slots.size())) {
        return;
    }
    size_t idx = size_t(handle.id);
    Slot s = slots[idx];
    if (s.active && s.generation == handle.generation) {
        callback(marbles[idx]);
    }
}

void MarblePool::update()
{
    count = 0;

    iterate([&](int index, Marble& marble) {
        marble.body.verletUpdate();
        marble.collider.center = marble.body.getPosition();

        if (CollisionManager::ifAnyCollision(&marble.collider, CollisionType::PlayerChomp)) {
            slots[index].active = false;
            available_slots.push(index);
            // callback to score
            GameState::score(marble.id == Marble::Yellow ? 10 : 1);
        } else if (CollisionManager::ifAnyCollision(&marble.collider, CollisionType::LevelDrain)) {
            slots[index].active = false;
            available_slots.push(index);
            // callback to damage
            GameState::damage(1);
        } else {
            CollisionManager::addDynamic(marble.getCollisionObject());
            count++;
        }
    });

    struct CollisionAggregator {
        int collision_count = 0;
        Vec2 collision_center;
        Vec2 collision_normal;
        Vec2 velocity_offset;
    } aggregator;

    int collisionMask = CollisionType::LevelObject | CollisionType::MarbleObject | CollisionType::PlayerObject;
    iterate([&](int index, Marble& marble){
        Vec2 vel = marble.body.getVelocity();

        aggregator = CollisionAggregator();

        CollisionManager::forDynamicCollisions(marble.getCollisionObject(), collisionMask, [&aggregator, &vel](const CollisionObject& other, CollisionInfo info) {
            double dot = vel.dot(info.normal);
            aggregator.collision_center += info.contact_point;
            aggregator.collision_normal += info.normal;

            // elastic collision: subtract the component of this ball's velocity projected on the collision normal
            //                    and add the component of the other ball's velocity projected on the normal
            aggregator.velocity_offset += info.normal * (other.body->getVelocity().dot(info.normal) - dot);

            aggregator.collision_count++;
        });
        if (aggregator.collision_count > 0) {
            aggregator.collision_center /= double(aggregator.collision_count);
            aggregator.collision_normal /= double(aggregator.collision_count);
            marble.body.setPosition(aggregator.collision_center + aggregator.collision_normal * marble_radius);
            marble.body.setVelocity(vel + aggregator.velocity_offset / double(aggregator.collision_count));

            marble.collider.center = marble.body.getPosition();
        }

        // do the wall pass second to try to override balls pushing eachother through walls
        vel = marble.body.getVelocity();
        CollisionManager::forStaticCollisions(marble.getCollisionObject(), collisionMask, [&marble, &vel](const CollisionObject& other, CollisionInfo info) {
            double dot = vel.dot(info.normal);
            // ignore the wall when the ball is moving away from it
            if (dot < 0) {
                marble.body.setPosition(info.contact_point + info.normal * marble_radius);
                marble.body.setVelocity(vel + info.normal * dot * -2);
            }
        });
    });
}

void MarblePool::draw()
{
    DrawSettings settings;
    settings.setTiled(0, 2, 1);
    Window::setDrawSettings(settings);

    Window::bind("marble");

    iterate([&](Marble& marble) {
        Vec2 p = marble.body.getInterpolatedPosition();
        settings.tile_id = marble.id;
        Window::setDrawSettings(settings);
        Window::drawBound(p.x, p.y, 0);
    });
}

void MarblePool::clear()
{
    marbles.clear();
    slots.clear();
    available_slots = std::stack<size_t>();
    grow(64);
}
