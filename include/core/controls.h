// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#ifndef CONTROLS_H
#define CONTROLS_H

#include <string>
#include <initializer_list>


class Controls;
class UserInput {
    double stored_value = 0;
    int counter = 0;
    int prev_counter = 0;

    friend Controls;

public:
    // if this input was pressed on the current frame or not
    bool pressed() const;

    // if this input is being held on the current frame
    bool held() const;

    // if this input was released on the current frame
    bool released() const;

    // the value stored by this input
    //   axes: range [-min, max]
    //   buttons: 0 or 1
    double value() const;
};


////////////////////////////////////////////////////////////
// Controls
//  Bind arbitrary action identifiers by key identifiers. Valid key identifiers are:
//
//   Keyboard keys:    kb_right, kb_left, kb_up, kb_down,
//                     kb_return, kb_esc, kb_escape, kb_space, kb_f1
//
//   Alphanumerics:    a, b, c... 0, 1, 2...
//
//   Gamepad buttons:  pad_south, pad_east, pad_north, pad_west,
//                     pad_select, pad_start,
//                     pad_dpad_l, pad_dpad_r, pad_dpad_u, pad_dpad_d,
//                     pad_rb, pad_lb
//
//   Gamepad analogue: pad_lstick_l, pad_lstick_r, pad_lstick_u, pad_lstick_d
//                     pad_rstick_l, pad_rstick_r, pad_rstick_u, pad_rstick_d
//                     pad_rt, pad_lt
////////////////////////////////////////////////////////////
class System;
class Controls
{
    static void update();

    friend System;
public:
    static void bindButton(std::string action_id, std::initializer_list<std::string> keys);

    static void bindAxis(std::string action_id, double min, double max, std::initializer_list<std::string> low_keys, std::initializer_list<std::string> high_keys);

    static UserInput get(std::string action_id);
};

#endif // CONTROLS_H
