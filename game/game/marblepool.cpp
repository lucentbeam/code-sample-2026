#include "marblepool.h"

#include <algorithm>

#include "core/system.h"
#include "core/window.h"

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

void MarblePool::release(const Handle &handle)
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

void MarblePool::get(const Handle &handle, std::function<void (Marble&)> callback)
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

void MarblePool::update(const std::vector<Wall> &walls)
{
    auto body_it = marbles.begin();
    auto slot_it = slots.begin();
    int ct = 0;
    while(body_it != marbles.end() && slot_it != slots.end()) {
        if (slot_it->active) {
            body_it->body.verletUpdate();
            Vec2 pos = body_it->body.getPosition(false);
            body_it->body.setVelocity(body_it->body.getVelocity());

            for (const Wall &wall : walls) {
                WallNearestLoc wc = wall.getNearest(pos);
                if (wc.distance < 3 && wc.wall_location_to_point.dot(wc.wall_normal) < 0) {
                    Vec2 vel = body_it->body.getVelocity();
                    double dot = vel.dot(wc.wall_normal);
                    if (dot < 0) {
                        body_it->body.setPosition(wc.wall_location);
                        body_it->body.setVelocity(vel - wc.wall_normal * dot * 2);
                    }
                    body_it->id = 1;
                }
            }
        }
        body_it++;
        slot_it++;
        ct++;
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
            Vec2 p = body_it->body.getPosition(true);
            settings.tile_id = body_it->id;
            Window::setDrawSettings(settings);
            Window::drawBound(p.x, p.y, 0);
        }
        body_it++;
        slot_it++;
    }
}
