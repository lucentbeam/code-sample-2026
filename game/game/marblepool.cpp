#include "marblepool.h"

#include <algorithm>

#include "core/system.h"
#include "core/window.h"

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

MarblePool::MarblePool()
{

}

MarblePool::Handle MarblePool::create(double x, double y)
{
    if (availableCount() == 0) grow(64);

    size_t idx = available_slots.top();
    available_slots.pop();

    Handle h;
    h.id = int(idx);
    h.generation = ++slots[idx].generation;
    slots[idx].active = true;

    marbles[idx].body = PhysicsBody(x, y);

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

#include <iostream>
void MarblePool::update(const std::vector<Line>& walls, const std::vector<Circle>& objects, const std::vector<Rect>& hitboxes)
{
    auto body_it = marbles.begin();
    auto slot_it = slots.begin();
    int slot_idx = 0;

    auto increment = [&body_it, &slot_it, &slot_idx](){
        body_it++;
        slot_it++;
        slot_idx++;
    };

    Circle collider;
    collider.radius = marble_radius;

    while(body_it != marbles.end() && slot_it != slots.end()) {
        if (slot_it->active) {
            body_it->body.verletUpdate();
            collider.center = body_it->body.getPosition();
            body_it->body.setVelocity(body_it->body.getVelocity());

            bool destroy = false;
            for (const Rect& hb : hitboxes) {
                if (CollisionDetection::overlapsRect(collider, hb)) {
                    destroy = true;
                    break;
                }
            }
            if (destroy) {
                slot_it->active = false;
                available_slots.push(slot_idx);
                std::cout << "champed!" << slot_idx << std::endl;
                // do other stuff here
                increment();
                continue;
            }
            /* Note:
             *  These collision resolution algorithms are highly simplified for the scope of this sample.
             *  They do not account for previous positions, and therefore phasing is possible.
             *  In addition, they resolve collisions instantly rather that resolving multi-body
             *  collisions.
             */
            for (const Circle& object : objects) {
                CollisionInfo c = CollisionDetection::circleCircle(collider, object);
                if (c.collides) {
                    Vec2 vel = body_it->body.getVelocity();
                    double dot = vel.dot(c.normal);
                    body_it->body.setPosition(c.contact_point + c.normal * collider.radius);
                    if (dot < 0) {
                        body_it->body.setVelocity(vel - c.normal * dot * 2);
                    }
                    body_it->id = 1;
                }
            }
            for (const Line& wall : walls) {
                CollisionInfo c = CollisionDetection::circleLine(collider, wall);
                if (c.collides) {
                    Vec2 vel = body_it->body.getVelocity();
                    double dot = vel.dot(c.normal);
                    if (dot < 0) {
                        body_it->body.setPosition(c.contact_point + c.normal * collider.radius);
                        body_it->body.setVelocity(vel - c.normal * dot * 2);
                    }
                    body_it->id = 1;
                }
            }
        }
        increment();
    }
}

void MarblePool::draw()
{
    auto body_it = marbles.begin();
    auto slot_it = slots.begin();

    DrawSettings settings;
    settings.setTiled(0, 2, 1);
    Window::setDrawSettings(settings);

    Window::bind("marble");

    while(body_it != marbles.end() && slot_it != slots.end()) {
        if (slot_it->active) {
            Vec2 p = body_it->body.getInterpolatedPosition();
            settings.tile_id = body_it->id;
            Window::setDrawSettings(settings);
            Window::drawBound(p.x, p.y, 0);
        }
        body_it++;
        slot_it++;
    }
}
