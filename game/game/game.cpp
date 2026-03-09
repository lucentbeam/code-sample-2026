#include "game.h"

#include "core/window.h"
#include "core/controls.h"

#include "states.h"

#include "game/player.h"
#include "game/arena.h"

#include "game/collisionmanager.h"
#include "constants.h"

namespace {
    bool init = false;
    Player player;
    Arena arena;
}

void Game::go()
{
    if (!init) {
        player.initialize();
        arena.initialize();
        init = true;
    }

//    player.reset();
//    arena.reset();
}

void Game::update(FSM &fsm)
{

    if (Controls::get("action").pressed()) {
        fsm.go(GameoverScreen);
        init = false;
        return;
    }

    CollisionManager::startFrame();

    player.update();
    arena.update();

    player.resolveCollisions();
    arena.resolveCollisions();
}

void Game::draw(FSM &)
{
    arena.draw();
    player.draw();
}
