#include "physicsbody.h"

#include "core/system.h"
#include "constants.h"

Vec2 PhysicsBody::getPrevious() const
{
    return previous;
}

Vec2 PhysicsBody::getPosition() const
{
    return position;
}

Vec2 PhysicsBody::getInterpolatedPosition() const
{
    return System::frameInterpolate(previous, position);
}

void PhysicsBody::setPosition(const Vec2 &p)
{
    position = p;
}

Vec2 PhysicsBody::getVelocity() const
{
    return position - previous;
}

void PhysicsBody::setVelocity(const Vec2 &vel)
{
    previous = position - vel;
}

void PhysicsBody::move(const Vec2 &delta)
{
    previous = position;
    position += delta;
}

void PhysicsBody::verletUpdate()
{
    move(position - previous);
}

