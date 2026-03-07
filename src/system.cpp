#include "system.h"

#include <chrono>
#include <iostream>
#include <cmath>

#include <SDL2/SDL.h>

#include "window.h"
#include "audio.h"
#include "controls.h"

namespace {
    double delta_time() {
        static auto last = std::chrono::high_resolution_clock::now();
        auto current = std::chrono::high_resolution_clock::now();
        double delta = std::chrono::duration<double>(current - last).count();
        last = current;
        return delta;
    }


}

double System::interpolation_fraction = 0.0;
double System::physics_timestep = 0.0143;
bool System::s_running = false;

bool System::initialize(SystemSettings settings, UpdateHook *update, DrawHook *draw)
{
    update_callback = update;
    draw_callback = draw;
    s_running = true;

    if (settings.physics_timestep > 0) {
        physics_timestep = settings.physics_timestep;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) < 0) {
        std::cerr << "ERROR: " << SDL_GetError() << std::endl;
        quit();
        return false;
    }

    if (!Window::initialize(settings.window_title, settings.resolution_x, settings.resolution_y, settings.starting_window_scale) || !Audio::initialize()) {
        quit();
    }

    return s_running;
}

void System::run()
{
    double accumulator = 0.0;
    SDL_Event event;

    while (s_running) {

        accumulator += std::fmin(delta_time(), 0.5); // prefer physics slowdown over runaway situations

        while (accumulator > 0) {

            while (SDL_PollEvent(&event)) {
                switch (event.type)
                {
                case SDL_WINDOWEVENT_CLOSE:
                case SDL_QUIT:
                    quit();
                    break;
                default: break;
                }
            }
            Controls::update();
            update_callback();
            accumulator -= physics_timestep;
        }

        Window::clear();
        // pass in the fraction that the last physics frame overshot the current draw frame
        //  frac = -accumulator/dt; frac is on range [0,1)
        //  at frac==0, draw latest frame. at frac == 1, draw the previous frame
        interpolation_fraction = -accumulator / physics_timestep;
        draw_callback();
        Window::update();
    }

    SDL_Quit();
}

void System::quit()
{
    s_running = false;
}
