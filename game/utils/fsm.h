#ifndef FSM_H
#define FSM_H

#include <unordered_map>

class FSM;
class State {
public:
    virtual ~State() {}

    virtual void go() {}
    virtual void update(FSM&) = 0;
    virtual void draw(FSM&) = 0;
};

class FSM
{
    std::unordered_map<int, State*> m_states;
    State * m_current = nullptr;

    bool tryGet(int id, State* &state);
public:
    FSM(std::unordered_map<int, State*> states);

    void go(int id);
    State* get(int id);

    void update();
    void draw();
};

#endif // FSM_H
