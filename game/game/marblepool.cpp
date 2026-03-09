#include "marblepool.h"

#include <algorithm>

#include "core/system.h"
#include "core/window.h"

#include "game/collisionmanager.h"

#include "constants.h"

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

inline void MarblePool::iterate(std::function<void (int, PhysicsBody&)> callback)
{
    auto body_it = marbles.begin();
    auto slot_it = slots.begin();
    int slot_idx = 0;

    while (body_it != marbles.end() && slot_it != slots.end()) {
        if (slot_it->active) {
            callback(slot_idx, body_it->body);
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
    Circle collider(marble_radius);

    count = 0;

    iterate([&](int index, PhysicsBody& body) {
        body.verletUpdate();
        collider.center = body.getPosition();

        if (CollisionManager::hitsAny(PlayerChomp, collider)) {
            slots[index].active = false;
            available_slots.push(index);
        } else {
            CollisionManager::addBall(index, collider, body);
            count++;
        }
    });

    Circle collider_prev(marble_radius);

    struct CollisionAggregator {
        int collision_count = 0;
        Vec2 collision_center;
        Vec2 collision_normal;
        Vec2 velocity_offset;
    } aggregator;

    iterate([&](int index, PhysicsBody& body){
        collider.center = body.getPosition();
        collider_prev.center = body.getPrevious();

        Vec2 vel = body.getVelocity();

        aggregator = CollisionAggregator{0, Vec2(), Vec2(), Vec2()};

        CollisionManager::forBallCollisions(index, [&aggregator, &vel](const PhysicsBody& other, CollisionInfo info) {
            double dot = vel.dot(info.normal);
            aggregator.collision_center += info.contact_point;
            aggregator.collision_normal += info.normal;

            // elastic collision: subtract the component of this ball's velocity projected on the collision normal
            //                    and add the component of the other ball's velocity projected on the normal
            aggregator.velocity_offset += info.normal * (other.getVelocity().dot(info.normal) - dot);

            aggregator.collision_count++;
        });
        if (aggregator.collision_count > 0) {
            aggregator.collision_center /= double(aggregator.collision_count);
            aggregator.collision_normal /= double(aggregator.collision_count);
            body.setPosition(aggregator.collision_center + aggregator.collision_normal * marble_radius);
            body.setVelocity(vel + aggregator.velocity_offset / double(aggregator.collision_count));

            collider.center = body.getPosition();
            collider_prev.center = body.getPrevious();
        }

        // do the wall pass second to try to override balls pushing eachother through walls
        vel = body.getVelocity();
        CollisionManager::forWallCollisions(collider_prev, collider, [&body, &vel](CollisionInfo info) {
            double dot = vel.dot(info.normal);
            // ignore the wall when the ball is moving away from it
            if (dot < 0) {
                body.setPosition(info.contact_point + info.normal * marble_radius);
                body.setVelocity(vel + info.normal * dot * -2);
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
