#include "game.h"

#include "core/window.h"
#include "core/controls.h"

#include "states.h"

#include "game/player.h"
#include "game/marblepool.h"

#include "game/collisiondetection.h"

#include "constants.h"

namespace {
    Player player;

    MarblePool marbles;

    std::vector<Line> walls;

    bool init = false;
}

void Game::update(FSM &fsm)
{
    if (!init) {
        player.init();

        constexpr int wallcount = 20;
        constexpr double step_size = 360 / wallcount;
        constexpr Vec2 center(arena_cx, arena_cy);
        for(int i = 0; i < 20; ++i) {
            Vec2 v1 = center + Vec2(double(i + 0) * step_size - 1) * arena_radius;
            Vec2 v2 = center + Vec2(double(i + 1) * step_size + 1) * arena_radius;
            walls.push_back(Line(v1, v2));
        }

        init = true;

        for(int i = 0; i < 40; ++i) {
            Vec2 p = center + Vec2(double(rand() % 360)) * (arena_radius * 0.25 + double(rand() % int(arena_radius * 0.5)));
            MarblePool::Handle h = marbles.create(p.x, p.y);
            marbles.get(h, [](Marble &m){
                const double speed = 3.0;
                double vx = rand() % 100;
                vx = ((vx / 100) - 0.5) * marble_speed * speed;
                double vy = rand() % 100;
                vy = ((vy / 100) - 0.5) * marble_speed * speed;
                m.body.move(Vec2(vx, vy));
            });
        }
    }
    if (Controls::get("action").pressed()) {
        fsm.go(GameoverScreen);
        return;
    }

    player.update();

    std::vector<Circle> circles;
    circles.push_back(player.getCollider());
    marbles.update(walls, circles, player.getHitBoxes());
}

void Game::draw(FSM &)
{
    Window::print("gameplay", 0, 0);

    marbles.draw();
    player.draw();
}
