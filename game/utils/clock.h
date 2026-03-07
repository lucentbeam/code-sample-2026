#ifndef CLOCK_H
#define CLOCK_H

#include <functional>

class Clock
{
    std::function<void()> m_callback;

    double m_duration;
    double m_max_duration;

    bool m_loops;
public:
    Clock(double duration, bool loops = false, std::function<void()> callback = nullptr);

    void bind(std::function<void()> callback);

    void update(double dt);

    bool done() const;

    void reset();
};

#endif // CLOCK_H
