// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>

typedef void UpdateHook();
typedef void DrawHook();

struct SystemSettings {
    std::string window_title;
    int resolution_x, resolution_y;
    double starting_window_scale;

    double physics_timestep;
};

class System
{
    static double interpolation_fraction, physics_timestep;
    static bool s_running;

    UpdateHook* update_callback;
    DrawHook* draw_callback;

public:
    bool initialize(SystemSettings settings, UpdateHook* update, DrawHook* draw);
    void run();

    static void quit();

    template<class C>
    static C frameInterpolate(C prev, C next);
};

template<class C>
inline C System::frameInterpolate(C prev, C next)
{
    return prev * interpolation_fraction + next * (1 - interpolation_fraction);
}

#endif // SYSTEM_H
