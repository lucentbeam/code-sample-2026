#ifndef MARBLEPOOL_H
#define MARBLEPOOL_H

#include <vector>
#include <stack>
#include <functional>

#include "game/physicsbody.h"
#include "game/marble.h"
#include "game/wall.h"

#include "game/collisiondetection.h"

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

public:
    MarblePool();

    class Handle {
        int id = -1;
        size_t generation = 0;
        friend MarblePool;
    };

    Handle create(double x, double y);

    void release(const Handle& handle);

    void get(const Handle& handle, std::function<void(Marble&)> callback);

    void update(const std::vector<Line>& walls, const std::vector<Circle>& objects, const std::vector<Rect>& hitboxes);

    void draw();
};

#endif // MARBLEPOOL_H
