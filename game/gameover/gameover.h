#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "utils/fsm.h"

class GameOver : public State
{
public:

    // State interface
    void update(FSM &) override;
    void draw(FSM &) override;
};

#endif // GAMEOVER_H
