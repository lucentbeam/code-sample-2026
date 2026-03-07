#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "core/utility.h"
#include "game/physicsbody.h"

#include "game/collisiondetection.h"

class Player
{
    PhysicsBody body;
    mutable Circle collider;

public:
    void init();

    void update();
    void draw();

    Circle getCollider() const;
};

#endif // PLAYER_H
