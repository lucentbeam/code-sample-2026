#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "core/utility.h"
#include "game/physicsbody.h"

#include "game/collisiondetection.h"
#include "utils/animation.h"

class Player
{
    AnimatedSprite m_sprite;
    PhysicsBody m_body;

public:
    void initialize();

    void update();

    void resolveCollisions();

    void draw();
};

#endif // PLAYER_H
