#include "animation.h"

Animation::Animation() :
    m_frames{0},
    m_duration(1),
    m_looping(false),
    m_start(std::chrono::steady_clock::now())
{

}

Animation::Animation(std::initializer_list<int> frames, double duration, bool loops) :
    m_frames(frames),
    m_duration(duration),
    m_looping(loops),
    m_start(std::chrono::steady_clock::now())
{

}

void Animation::reset(double offset)
{
    m_start = std::chrono::steady_clock::now();

    // do not allow positive offsets
    offset = std::fmax(0, offset);
    if (offset > 0) {
        auto offset_duration = std::chrono::duration_cast<std::chrono::steady_clock::duration>(
            std::chrono::duration<double>(offset)
        );
        m_start -= offset_duration;
    }
}

int Animation::frame() const
{
    double frac = std::chrono::duration<double>(std::chrono::steady_clock::now() - m_start).count() / m_duration;

    if (m_looping) {
        frac = std::fmod(frac, 1);
    } else {
        frac = std::fmin(frac, 0.9999);
    }
    return m_frames[size_t(frac * double(m_frames.size()))];
}

bool Animation::done() const
{
    return doneDuration() >= 0;
}

double Animation::doneDuration() const
{
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - m_start).count() - m_duration;
}

void AnimatedSprite::bind(int id, const Animation& animation)
{
    m_animations.emplace(id, AnimationState());
    m_animations[id].animation = animation;
    m_animations[id].auto_transition = false;

    if (m_animations.size() == 1) m_current = id;
}

void AnimatedSprite::bind(int id, const Animation& animation, int fallback_on_done)
{
    m_animations.emplace(id, AnimationState());
    m_animations[id].animation = animation;
    m_animations[id].auto_transition = fallback_on_done != id;
    m_animations[id].transitions_to = fallback_on_done;

    if (m_animations.size() == 1) m_current = id;
}

void AnimatedSprite::go(int state, bool reset_if_same)
{
    if (state == m_current && !reset_if_same) return;
    auto it = m_animations.find(state);
    if (it == m_animations.end()) {
        return;
    }
    m_current = state;
    it->second.animation.reset();
}

int AnimatedSprite::currentState()
{
    AnimationState state = m_animations.at(m_current);
    if (state.auto_transition && state.animation.done()) {
        double overflow = state.animation.doneDuration();
        m_current = state.transitions_to;
        state = m_animations.at(m_current);
        state.animation.reset(overflow);
    }
    return m_current;
}

int AnimatedSprite::frame()
{
    if (m_animations.size() == 0) return 0;

    return m_animations.at(currentState()).animation.frame();
}
