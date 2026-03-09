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
