// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "core/utility.h"

#include "game/marblepool.h"

#include "utils/clock.h"

class Launcher
{
    MarblePool * m_marbles;

    Vec2 m_position;
    Vec2 m_facing;
    Clock m_timer;

    double m_accumulator = 0;

    unsigned int m_shot_counter;

    Vec2 facing() const;
public:
    Launcher(MarblePool * pool, Vec2 pos, Vec2 dir);

    void update();
    void draw();
};

#endif // LAUNCHER_H
