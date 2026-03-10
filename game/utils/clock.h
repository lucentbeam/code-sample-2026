// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#ifndef CLOCK_H
#define CLOCK_H

class Clock
{
    double m_duration;
    double m_max_duration;

    bool m_loops;
public:
    Clock(double duration, bool loops = false);

    bool update(double dt);

    bool done() const;

    void reset();
};

#endif // CLOCK_H
