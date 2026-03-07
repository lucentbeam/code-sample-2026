#ifndef PLAYER_H
#define PLAYER_H

#include "core/utility.h"
#include "game/physicsbody.h"

class Player
{
    PhysicsBody body;

public:
    void init();

    void update();
    void draw();
};

#endif // PLAYER_H
