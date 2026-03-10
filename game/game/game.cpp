// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#include "game.h"

#include "core/window.h"
#include "core/controls.h"

#include "states.h"

#include "game/player.h"
#include "game/arena.h"

#include "game/collisionmanager.h"
#include "constants.h"
#include "game/gamestate.h"

namespace {
    bool game_active = false;
    Player player;
    Arena arena;
}

void Game::go()
{
    GameState::reset();

    player.initialize();
    arena.initialize();
    game_active = true;

    Window::setClearColor(game_screen_clear_color);
}

void Game::update(FSM &fsm)
{
    if (GameState::gameover()) {
        fsm.go(GameoverScreen);
        game_active = false;
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

    if (!game_active) return;

    std::string txt = "SCORE: " + std::to_string(GameState::score());
    Window::print(txt, 4, 4);

    txt = "HEALTH: " + std::to_string(GameState::health());
    Window::print(txt, 4, resolution_y - 12);

}
