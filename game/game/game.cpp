#include "game.h"

#include "core/window.h"
#include "core/controls.h"

#include "states.h"

#include "game/player.h"
#include "game/marblepool.h"
#include "game/launcher.h"

#include "game/collisiondetection.h"

#include "constants.h"

namespace {
    Player player;

    MarblePool marbles;

    std::vector<Line> walls;

    Launcher launcher(&marbles, Vec2(arena_cx, arena_cy - arena_radius), Vec2(0, 1));

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

        launcher.start();

        init = true;
    }
    if (Controls::get("action").pressed()) {
        fsm.go(GameoverScreen);
        return;
    }

    launcher.update();
    player.update();

    std::vector<Circle> circles;
    circles.push_back(player.getCollider());
    marbles.update(walls, circles, player.getHitBoxes());
}

void Game::draw(FSM &)
{
    Window::print("gameplay", 0, 0);

    launcher.draw();
    marbles.draw();
    player.draw();
}
