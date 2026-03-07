#include "physicsbody.h"

#include "core/system.h"
#include "constants.h"

Vec2 PhysicsBody::getPosition(bool interpolated) const
{
    if (interpolated) {
        return System::frameInterpolate(previous, position);
    } else {
        return position;
    }
}

void PhysicsBody::setPosition(const Vec2 &p)
{
    Vec2 delta = p - position;
    position = p;
    previous += delta;
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

