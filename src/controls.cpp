// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#include "controls.h"

#include <cmath>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <memory>

#include "utilities/platformcontrols.h"

namespace {

    class Button : public InputChecker {
        std::vector<InputChecker*> inputs;

    public:
        Button(std::vector<InputChecker*> inputs) : inputs(inputs) {}

        double value() const override {
            double v = 0;
            for (InputChecker *input : inputs) {
                v = std::fmax(v, input->value());
            }
            return v;
        }
    };

    class Axis : public InputChecker {
        Button inputs_low, inputs_high;
        double min, max;


    public:
        Axis(double min, double max, std::vector<InputChecker*> low, std::vector<InputChecker*> high) :
            inputs_low(low),
            inputs_high(high),
            min(min),
            max(max)
        {}

        double value() const override {
            return min * inputs_low.value() + max * inputs_high.value();
        }

        double deadzone() const override { return 0.3; }
    };

    // store the Digital & Analogue resources created as references to the user's interactions
    std::unordered_map<std::string, InputChecker*> bound_keys;

    std::vector<InputChecker*> getKeys(std::initializer_list<std::string> keys) {
        std::vector<InputChecker*> inputs;
        for (std::string key : keys) {
            auto it = bound_keys.find(key);
            if (it != bound_keys.end()) {
                inputs.push_back(it->second);
            } else {
                InputChecker* inpt = PlatformControls::createInput(key);
                if (inpt == nullptr) {
                    std::cerr << "ERROR: No key id for " << key << " on the current platform bindings" << std::endl;
                } else {
                    inputs.push_back(inpt);
                    bound_keys.emplace(key, inpt);
                }
            }
        }
        return inputs;
    }

    // store the Button & Axis resources created as maps to game actions
    std::unordered_map<std::string, std::unique_ptr<InputChecker>> bound_actions;

    void checkAndRemove(std::string id) {
        auto it = bound_actions.find(id);
        if (it != bound_actions.end()) {
            std::cout << "INFO: Overwriting bound action named " << id << std::endl;
            bound_actions.erase(it);
        }
    }
}

bool UserInput::pressed() const { return counter == 1; }

bool UserInput::held() const { return counter > 0; }

bool UserInput::released() const { return counter == 0 && prev_counter > 0; }

double UserInput::value() const { return stored_value; }

void Controls::update()
{
    PlatformControls::update();
    for (auto &kvp : bound_actions) {
        kvp.second->update();
    }
}

UserInput Controls::get(std::string id)
{
    UserInput inpt;
    auto it = bound_actions.find(id);
    if (it != bound_actions.end()) {
        inpt.counter = it->second->counts();
        inpt.prev_counter = it->second->previous_counts();
        inpt.stored_value = it->second->value();
    } else {
        std::cout << "WARNING: No user input bound to id " << id << std::endl;
    }
    return inpt;
}

void Controls::bindButton(std::string id, std::initializer_list<std::string> keys)
{
    checkAndRemove(id);
    bound_actions.emplace(id, std::unique_ptr<InputChecker>(new Button(getKeys(keys))));
}

void Controls::bindAxis(std::string id, double min, double max, std::initializer_list<std::string> low_keys, std::initializer_list<std::string> high_keys)
{
    checkAndRemove(id);
    bound_actions.emplace(id, std::unique_ptr<InputChecker>(new Axis(min, max, getKeys(low_keys), getKeys(high_keys))));
}
