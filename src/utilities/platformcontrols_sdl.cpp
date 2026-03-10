// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#include "platformcontrols.h"

#include <unordered_map>
#include <ctype.h>
#include <limits>

#include <SDL2/SDL.h>

namespace {
    const uint8_t * keystate = nullptr;

    class Digital : public InputChecker {
        union {
            SDL_Scancode code;
            SDL_GameControllerButton button;
        } input;
        bool keyboard;

    public:
        Digital(SDL_Scancode code) : keyboard(true) { input.code = code; }
        Digital(SDL_GameControllerButton button) : keyboard(false) { input.button = button; }

        double value() const override {
            if (keyboard) {
                return keystate != nullptr && keystate[input.code] ? 1 : 0;
            } else {
                for(int i = 0; i < 16; ++i) {
                    if (SDL_IsGameController(i)) {
                        SDL_GameController * joy = SDL_GameControllerOpen(i);
                        if (SDL_GameControllerGetAttached(joy)) {
                            if (SDL_GameControllerGetButton(joy, input.button)) {
                                return true;
                            }
                        }
                    }
                }
                return false;
            }
        }

        double deadzone() const override { return std::numeric_limits<double>::epsilon(); }
    };

    class Analogue : public InputChecker {
        double lower, upper;
        SDL_GameControllerAxis axis;
    public:
        Analogue(SDL_GameControllerAxis id, double lower, double upper) : lower(lower), upper(upper), axis(id) {}

        double value() const override {
            double dy = upper - lower;
            double bottom = lower;
            double f = 0;
            for(int jid = 0; jid < 16; ++jid) {
                SDL_GameController * joy = SDL_GameControllerOpen(jid);
                if (SDL_GameControllerGetAttached(joy)) {
                    double val = double(SDL_GameControllerGetAxis(joy, axis)) / 32767;
                    f = std::fmax(f, (val - bottom)/dy);
                }
            }
            return f;
        }

        double deadzone() const override { return 0.3; }
    };
}

InputChecker *PlatformControls::createInput(std::string id)
{
    if (id.size() == 1) {
        if (isalpha(id[0])) {
            SDL_Scancode code = SDL_Scancode(int(SDL_SCANCODE_A) + (toupper(id[0]) - 'A'));
            return new Digital(code);
        } else if (isdigit(id[0])) {
            SDL_Scancode code = SDL_Scancode(int(SDL_SCANCODE_0) + (id[0] - '0'));
            return new Digital(code);
        }
    }
    if (id.substr(0, 3) == "kb_") {
        std::unordered_map<std::string, SDL_Scancode> codes = {
            {"kb_right", SDL_SCANCODE_RIGHT},
            {"kb_left", SDL_SCANCODE_LEFT},
            {"kb_up", SDL_SCANCODE_UP},
            {"kb_down", SDL_SCANCODE_DOWN},
            {"kb_return", SDL_SCANCODE_RETURN},
            {"kb_esc", SDL_SCANCODE_ESCAPE},
            {"kb_escape", SDL_SCANCODE_ESCAPE},
            {"kb_space", SDL_SCANCODE_SPACE},
            {"kb_f1", SDL_SCANCODE_F1}
        };
        if (codes.find(id) != codes.end()) {
            return new Digital(codes.at(id));
        }
    } else if (id.substr(0, 4) == "pad_") {
        id = id.substr(4, id.length() - 4);

        std::unordered_map<std::string, SDL_GameControllerButton> codes = {
            {"south", SDL_CONTROLLER_BUTTON_A},
            {"east", SDL_CONTROLLER_BUTTON_B},
            {"west", SDL_CONTROLLER_BUTTON_X},
            {"north", SDL_CONTROLLER_BUTTON_Y},
            {"select", SDL_CONTROLLER_BUTTON_BACK},
            {"start", SDL_CONTROLLER_BUTTON_START},
            {"dpad_l", SDL_CONTROLLER_BUTTON_DPAD_LEFT},
            {"dpad_r", SDL_CONTROLLER_BUTTON_DPAD_RIGHT},
            {"dpad_u", SDL_CONTROLLER_BUTTON_DPAD_UP},
            {"dpad_d", SDL_CONTROLLER_BUTTON_DPAD_DOWN},
            {"rb", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
            {"lb", SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
        };
        if (codes.find(id) != codes.end()) {
            return new Digital(codes.at(id));
        }

        auto matches_id = [&id](std::string l) {
            return id.rfind(l, 0) == 0;
        };
        if (matches_id("lstick_l")) {
            return new Analogue(SDL_CONTROLLER_AXIS_LEFTX, 0,-1);
        } else if (matches_id("lstick_r")) {
            return new Analogue(SDL_CONTROLLER_AXIS_LEFTX, 0, 1);
        } else if (matches_id("lstick_u")) {
            return new Analogue(SDL_CONTROLLER_AXIS_LEFTY, 0,-1);
        } else if (matches_id("lstick_d")) {
            return new Analogue(SDL_CONTROLLER_AXIS_LEFTY, 0, 1);
        } else if (matches_id("rstick_l")) {
            return new Analogue(SDL_CONTROLLER_AXIS_RIGHTX, 0,-1);
        } else if (matches_id("rstick_r")) {
            return new Analogue(SDL_CONTROLLER_AXIS_RIGHTX, 0, 1);
        } else if (matches_id("rstick_u")) {
            return new Analogue(SDL_CONTROLLER_AXIS_RIGHTY, 0,-1);
        } else if (matches_id("rstick_d")) {
            return new Analogue(SDL_CONTROLLER_AXIS_RIGHTY, 0, 1);
        } else if (matches_id("rt")) {
            return new Analogue(SDL_CONTROLLER_AXIS_TRIGGERRIGHT, 0, 1);
        } else if (matches_id("lt")) {
            return new Analogue(SDL_CONTROLLER_AXIS_TRIGGERLEFT, 0, 1);
        }
    }
    return nullptr;
}

void PlatformControls::update()
{
    SDL_JoystickUpdate();
    keystate = SDL_GetKeyboardState(nullptr);
}
