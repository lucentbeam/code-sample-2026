#include "game.h"

#include "core/window.h"
#include "core/controls.h"

#include "states.h"

#include "game/player.h"
#include "game/marblepool.h"
#include "game/launcher.h"

#include "game/collisionmanager.h"

#include "constants.h"

namespace {
    Player player;

    MarblePool marbles;

    Launcher launcher(&marbles, Vec2(arena_cx, arena_cy - arena_radius), Vec2(0, 1));

    Rect arena = {Vec2(arena_cx, arena_cy) - Vec2(2,2) * arena_radius, Vec2(arena_cx, arena_cy) + Vec2(2,2) * arena_radius};
    bool init = false;
}

void Game::update(FSM &fsm)
{
    if (!init) {
        CollisionManager::initialize(arena, Vec2(16,16));
        player.init();

        constexpr int wallcount = 20;
        constexpr double step_size = 360 / wallcount;
        constexpr Vec2 center(arena_cx, arena_cy);
        for(int i = 0; i < 20; ++i) {
            Vec2 v1 = center + Vec2(double(i + 0) * step_size - 1) * arena_radius;
            Vec2 v2 = center + Vec2(double(i + 1) * step_size + 1) * arena_radius;
            CollisionManager::addWall(Line(v1, v2));
        }

        launcher.start();

        init = true;
    }

    if (Controls::get("action").pressed()) {
        fsm.go(GameoverScreen);
        return;
    }

    CollisionManager::clearBalls();

    launcher.update();
    player.update();
    marbles.update(player.getHitBoxes());
}

void Game::draw(FSM &)
{
    char buf[30];
    std::sprintf(buf, "marble count: %d", marbles.count);
    Window::print(buf, 0, 0);

    launcher.draw();
    marbles.draw();
    player.draw();
}
