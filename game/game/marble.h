#ifndef MARBLE_H
#define MARBLE_H

#include <cmath>

#include "game/physicsbody.h"
#include "game/collisionmanager.h"

struct Marble {
    PhysicsBody body;

    int id = 0;
};

#endif // MARBLE_H
