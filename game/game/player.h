// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
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
    Circle m_collider;
    Circle m_mouth_colliders[4];

public:
    void initialize();

    void update();

    void resolveCollisions();

    void draw();
};

#endif // PLAYER_H
