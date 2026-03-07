#include "clock.h"

Clock::Clock(double duration, bool loops, std::function<void ()> callback) :
    m_callback(callback),
    m_duration(duration),
    m_max_duration(duration),
    m_loops(loops)
{

}

void Clock::bind(std::function<void ()> callback)
{
    m_callback = callback;
}

void Clock::update(double dt)
{
    if (done()) return;
    m_duration -= dt;
    if (done() && m_callback != nullptr) {
        m_callback();
        if (m_loops) {
            m_duration += m_max_duration;
        }
    }
}

bool Clock::done() const
{
    return m_duration <= 0;
}

void Clock::reset()
{
    m_duration = m_max_duration;
}
