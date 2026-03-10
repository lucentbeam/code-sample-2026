// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#include "title.h"

#include "core/window.h"
#include "core/controls.h"

#include "states.h"
#include "constants.h"

void Title::go()
{
    Window::setClearColor(0);
}

void Title::update(FSM &fsm)
{
    if (Controls::get("action").released()) {
        fsm.go(GameScreen);
        return;
    }
}

void Title::draw(FSM &)
{
    Window::setDrawSettings();
    Window::print("Hippos of Significant Hunger", 0, 30, 2);

    DrawSettings settings;
    settings.color = Controls::get("action").held() ? ColorLightGreen : 0xffffff;
    Window::setDrawSettings(settings);

    Window::print("<space/return> to play", resolution_x * 0.25, resolution_y/2 + 20, 0.75);
}
