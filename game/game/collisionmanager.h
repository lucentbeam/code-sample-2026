#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <vector>
#include <functional>

#include "collisiondetection.h"
#include "utils/shapes.h"
#include "utils/grid.h"
#include "game/physicsbody.h"

enum CollisionType {
    LevelObject = 1 << 0,
    PlayerObject = 1 << 1,
    MarbleObject = 1 << 2,
    PlayerChomp = 1 << 3,
    LevelDrain = 1 << 4
};

struct CollisionObject {
    CollisionType type;
    Collider * collider;
    PhysicsBody * body;
};

// Holds statics (stored once) and dynamics (stored per-frame) for checking collider overlaps
class CollisionManager
{
    struct CollisionObjectContainer {
        std::vector<CollisionObject> entries;
        CollisionGrid grid;

        size_t add(const CollisionObject &c) {
            size_t internal_id = entries.size();
            grid.insert(internal_id, c.collider->getBounds());
            entries.push_back(c);
            return internal_id;
        }

        void clear() {
            entries.clear();
            grid.clear();
        }
    };

    static CollisionObjectContainer s_statics;
    static CollisionObjectContainer s_dynamics;

    inline static void forCollisions(const CollisionObjectContainer& container, const CollisionObject& obj, int mask, std::function<void(const CollisionObject&, CollisionInfo)> callback);
public:
    // functions called during initialization
    static void initialize(const Rect& bounds, size_t horizontal_subdivisions, size_t vertical_subdivisions);
    static void addStatic(CollisionObject obj);

    // functions called before every physics frame
    static void startFrame();

    // functions called during physics frames
    static void addDynamic(CollisionObject obj);

    static bool ifAnyCollision(Collider* collider, int mask);

    static void forStaticCollisions(const CollisionObject& obj, int mask, std::function<void(const CollisionObject&, CollisionInfo)> callback);
    static void forDynamicCollisions(const CollisionObject& obj, int mask, std::function<void(const CollisionObject&, CollisionInfo)> callback);
};

#endif // COLLISIONMANAGER_H
