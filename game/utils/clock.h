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
