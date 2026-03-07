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
    mutable Circle m_collider;

public:
    void init();

    void update();
    void draw();

    Circle getCollider() const;
};

#endif // PLAYER_H
