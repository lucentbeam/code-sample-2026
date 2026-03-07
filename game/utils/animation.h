#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <unordered_map>
#include <chrono>

class Animation
{
    std::vector<int> m_frames;
    double m_duration;

    bool m_looping;

    std::chrono::steady_clock::time_point m_start;
public:
    Animation();
    Animation(std::initializer_list<int> frames, double over_duration, bool loops);

    void reset(double already_run_for = 0);

    int frame() const;

    bool done() const;

    double doneDuration() const;

    double fraction() const;
};

class AnimatedSprite
{
    struct AnimationState {
        Animation animation;
        int transitions_to;
        bool auto_transition = false;
    };

    std::unordered_map<int, AnimationState> m_states;

    int m_current;
public:
    void bind(int id, const Animation &animation);
    void bind(int id, const Animation &animation, int fallback_on_done);

    // goes to a state. if it's the current state, ignore it unless reset_if_same is set
    void go(int state, bool reset_if_same = false);

    int currentState();
    int frame();
    double fraction() const;
};



#endif // ANIMATION_H
