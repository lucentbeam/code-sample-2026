// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#ifndef GAME_H
#define GAME_H

#include "utils/fsm.h"

class Game : public State
{

public:

    // State interface
    void go() override;
    void update(FSM &) override;
    void draw(FSM &) override;
};

#endif // GAME_H
