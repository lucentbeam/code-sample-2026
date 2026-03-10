// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#include "clock.h"

Clock::Clock(double duration, bool loops) :
    m_duration(duration),
    m_max_duration(duration),
    m_loops(loops)
{

}

bool Clock::update(double dt)
{
    if (done() && !m_loops) return true;
    m_duration -= dt;
    if (done() && m_loops) {
        m_duration += m_max_duration;
        return true;
    }
    return done();
}

bool Clock::done() const
{
    return m_duration <= 0;
}

void Clock::reset()
{
    m_duration = m_max_duration;
}
