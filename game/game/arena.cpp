#include "arena.h"

#include "utils/shapes.h"
#include "constants.h"
#include "game/collisionmanager.h"
#include "core/window.h"

namespace {
    Rect arena = {Vec2(arena_cx, arena_cy) - Vec2(2,2) * arena_radius, Vec2(arena_cx, arena_cy) + Vec2(2,2) * arena_radius};

    Vec2 wall_start;
    Vec2 wall_last;
}

void Arena::addLauncher(Vec2 at, Vec2 facing)
{
    m_launchers.push_back(Launcher(&marbles, at, facing));
}

void Arena::addDrain(Vec2 at)
{
    m_drains.push_back(m_circles.size());
    m_circles.push_back(Circle(at, drain_radius));
}

void Arena::addPost(Vec2 at)
{
    m_posts.push_back(m_circles.size());
    m_circles.push_back(Circle(at, post_radius));
}

void Arena::startWall(Vec2 at)
{
    wall_start = wall_last = at;
}

void Arena::drawWallTo(Vec2 to)
{
    m_walls.push_back(Line(wall_last, to));
    wall_last = to;
}

void Arena::closeWall()
{
    drawWallTo(wall_start);
}

void Arena::initialize()
{
    CollisionManager::initialize(arena, 8, 8);

    // todo: load this from json or preferably svg to support tooling

    constexpr int wallcount = 20;
    constexpr double step_size = 360 / wallcount;
    constexpr Vec2 center(arena_cx, arena_cy);
    startWall(center + Vec2(0) * arena_radius);
    for(int i = 1; i < 20; ++i) {
        drawWallTo(center + Vec2(double(i) * step_size) * arena_radius);
    }
    closeWall();

    addLauncher({arena_cx + arena_radius * 0.43, arena_radius * 0.25}, {-1, 1});
    addLauncher({arena_cx - arena_radius * 0.43, arena_radius * 0.25}, {1, 1});

    addPost({arena_cx + arena_radius * 0.3, arena_cy + arena_radius * 0.15});
    addPost({arena_cx - arena_radius * 0.3, arena_cy + arena_radius * 0.15});

    addDrain({arena_cx, arena_cy + arena_radius * 0.66});

    // push level elements as statics
    CollisionObject obj = {CollisionType::LevelObject, nullptr, nullptr};
    for (size_t idx : m_posts) {
        obj.collider = &*(m_circles.begin() + idx);
        CollisionManager::addStatic(obj);
    }
    for (Line& wall : m_walls) {
        obj.collider = &wall;
        CollisionManager::addStatic(obj);
    }
    obj.type = CollisionType::LevelDrain;
    for (size_t idx : m_drains) {
        obj.collider = &*(m_circles.begin() + idx);
        CollisionManager::addStatic(obj);
    }
}

void Arena::update()
{
    for (Launcher &launcher : m_launchers) {
        launcher.update();
    }
}

void Arena::resolveCollisions()
{
    marbles.update();
}

void Arena::draw()
{
    DrawSettings settings;
    settings.y_sorted = false;
    Window::setDrawSettings(settings);
    Window::draw("arena", arena_cx, arena_cy, 0);
    for(size_t idx : m_drains) {
        const Circle& drain = m_circles[idx];
        Window::draw("drain", drain.center.x, drain.center.y, 0);
    }

    char buf[30];
    std::sprintf(buf, "marble count: %d", marbles.count);
    Window::print(buf, 0, 0);

    Window::setDrawSettings();
    for(size_t idx : m_posts) {
        const Circle& post = m_circles[idx];
        Window::draw("post", post.center.x, post.center.y, 0);
    }

    for(Launcher &launcher : m_launchers) launcher.draw();
    marbles.draw();
}
