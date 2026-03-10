#ifndef TITLE_H
#define TITLE_H

#include "utils/fsm.h"

class Title : public State
{
public:

    // State interface
    void go() override;
    void update(FSM &) override;
    void draw(FSM &) override;
};

#endif // TITLE_H
