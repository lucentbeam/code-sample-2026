// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#include <iostream>

#include "core/system.h"

#include "manager.h"
#include "constants.h"

int main()
{
    SystemSettings settings;
    settings.physics_timestep = physics_timestep;
    settings.window_title = window_title;
    settings.resolution_x = resolution_x;
    settings.resolution_y = resolution_y;
    settings.starting_window_scale = starting_window_scale;

    System system;
    if (!system.initialize(settings, &Manager::update, &Manager::draw)) {
        return -1;
    }

    Manager::initialize();

    system.run();
    return 0;
}
