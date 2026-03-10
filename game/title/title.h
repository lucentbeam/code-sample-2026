// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#ifndef TITLE_H
#define TITLE_H

#include "utils/fsm.h"

class Title : public State
{
public:

    // State interface
    void go() override;
    void update(FSM &) override;
    void draw(FSM &) override;
};

#endif // TITLE_H
