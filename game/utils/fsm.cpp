#include "fsm.h"

#include <iostream>

bool FSM::tryGet(int id, State *&state)
{
    auto it = m_states.find(id);
    if (it != m_states.end()) {
        state = it->second;
        return true;
    }
    return false;
}

FSM::FSM(std::unordered_map<int, State *> states) :
    m_states(states),
    m_current(nullptr)
{

}

void FSM::go(int id)
{
    if (!tryGet(id, m_current)) {
        std::cerr << "ERROR: State number " << id << " does not exist" << std::endl;
        return;
    } else {
        m_current->go();
    }
}


State *FSM::get(int id)
{
    State* result = nullptr;
    if (!tryGet(id, result)) {
        std::cerr << "ERROR: State number " << id << " does not exist" << std::endl;
    }
    return result;
}

void FSM::update()
{
    if (m_current == nullptr) return;
    m_current->update(*this);
}

void FSM::draw()
{
    if (m_current == nullptr) return;
    m_current->draw(*this);
}
