// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#ifndef CONTROLS_SDL_H
#define CONTROLS_SDL_H

#include <cmath>
#include <string>

class InputChecker {
    int counter = 0;
    int prev_counter = 0;

public:
    virtual ~InputChecker() {}

    void update() {
        prev_counter = counter;
        if (std::fabs(value()) > deadzone()) {
            ++counter;
        } else {
            counter = 0;
        }
    }

    int counts() const { return counter; }
    int previous_counts() const { return prev_counter; }
    virtual double value() const = 0;
    virtual double deadzone() const { return 0; }
};

struct PlatformControls
{
    static InputChecker * createInput(std::string id);

    static void update();
};

#endif // CONTROLS_SDL_H
