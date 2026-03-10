// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#ifndef PHYSICSBODY_H
#define PHYSICSBODY_H

#include "core/utility.h"

class PhysicsBody
{
    Vec2 previous;
    Vec2 position;

public:
    PhysicsBody(Vec2 pos) : previous(pos), position(pos) {}
    PhysicsBody(Vec2 pos, Vec2 vel) : previous(pos - vel), position(pos) {}
    PhysicsBody(double x = 0, double y = 0) : PhysicsBody(Vec2(x,y)) {}

    Vec2 getPrevious() const;
    Vec2 getPosition() const;
    Vec2 getInterpolatedPosition() const;
    void setPosition(const Vec2 &position);

    Vec2 getVelocity() const;
    void setVelocity(const Vec2 &vel);

    void move(const Vec2 &delta);

    void verletUpdate();
};

#endif // PHYSICSBODY_H
