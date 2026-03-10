// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#include "gameover.h"

#include "core/window.h"
#include "core/controls.h"

#include "game/gamestate.h"

#include "states.h"
#include "constants.h"

namespace {
    double delay_timer;
}

void GameOver::go()
{
    // add a little delay to keep people from mashing buttons past this screen
    delay_timer = 2.0;
}

void GameOver::update(FSM &fsm)
{
    delay_timer -= physics_timestep;
    if (delay_timer < 0 && Controls::get("action").released()) {
        fsm.go(TitleScreen);
        return;
    }
}

void GameOver::draw(FSM &fsm)
{
    Window::setDrawSettings();
    fsm.get(GameScreen)->draw(fsm);

    DrawSettings settings;
    settings.y_sorted = false;
    Window::setDrawSettings(settings);

    Window::print("GAMEOVER", resolution_x/2 - 50, resolution_y/2 - 12, 2);
    Window::print("final score: " + std::to_string(GameState::score()), resolution_x * 0.35, resolution_y/2 + 30);
}
