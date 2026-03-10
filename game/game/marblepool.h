#ifndef MARBLEPOOL_H
#define MARBLEPOOL_H

#include <vector>
#include <stack>
#include <functional>

#include "game/physicsbody.h"

#include "utils/shapes.h"
#include "constants.h"

struct CollisionObject;
struct Marble {
    Circle collider = Circle(marble_radius);

    PhysicsBody body;

    enum Type {
        Red = 0,
        Yellow = 1
    } id;

    CollisionObject getCollisionObject();
};

class MarblePool {
    struct Slot {
        size_t generation = 0;
        bool active = false;
    };

    std::vector<Marble> marbles;
    std::vector<Slot> slots;
    std::stack<size_t> available_slots;

    void grow(int count);

    size_t availableCount();

    inline void iterate(std::function<void(int, Marble&)> callback);

    inline void iterate(std::function<void(Marble&)> callback);
public:

    class Handle {
        int id = -1;
        size_t generation = 0;
        friend MarblePool;
    };

    Handle create(Vec2 position, Vec2 velocity, Marble::Type type);

    void release(const Handle& handle);

    void get(const Handle& handle, std::function<void(Marble&)> callback);

    void update();

    void draw();

    int count;
};

#endif // MARBLEPOOL_H
