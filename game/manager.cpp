// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#include "manager.h"

#include "core/controls.h"
#include "core/window.h"

#include "title/title.h"
#include "game/game.h"
#include "gameover/gameover.h"

#include "constants.h"
#include "states.h"

namespace {
    FSM fsm = FSM({
        {TitleScreen,       new Title()},
        {GameScreen,        new Game()},
        {GameoverScreen,    new GameOver()}
    });
}

void Manager::initialize()
{
    Controls::bindButton("action", {"kb_return", "pad_south", "pad_start", "kb_space"});
    Controls::bindAxis("horizontal", -1, 1, {"a", "kb_left", "pad_lstick_l", "pad_dpad_l"}, {"d", "kb_right", "pad_lstick_r", "pad_dpad_r"});
    Controls::bindAxis("vertical", -1, 1, {"w", "kb_up", "pad_lstick_u", "pad_dpad_d"}, {"s", "kb_down", "pad_lstick_d", "pad_dpad_d"});

    Window::setClearColor(game_screen_clear_color);

    fsm.go(TitleScreen);
}

void Manager::update()
{
    fsm.update();
}

void Manager::draw()
{
    fsm.draw();
}
