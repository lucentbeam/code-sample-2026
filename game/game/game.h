#ifndef GAME_H
#define GAME_H

#include "utils/fsm.h"

class Game : public State
{

public:

    // State interface
    void update(FSM &) override;
    void draw(FSM &) override;
};

#endif // GAME_H
