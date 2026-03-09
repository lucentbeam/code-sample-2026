#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <vector>
#include <functional>

#include "collisiondetection.h"
#include "utils/shapes.h"
#include "utils/grid.h"
#include "game/physicsbody.h"

enum HurtboxType {
    PlayerChomp,
    LevelDrain,
};

// Holds statics (stored once) and dynamics (stored per-frame) for checking collider overlaps
class CollisionManager
{

    template<class C>
    struct TypeContainer {
        std::vector<C> entries;
        CollisionGrid grid;

        size_t add(const C &c) {
            size_t internal_id = entries.size();
            grid.insert(int(internal_id), c.getBounds());
            entries.push_back(c);
            return internal_id;
        }

        void clear() {
            entries.clear();
            grid.clear();
        }
    };

    static TypeContainer<Line> s_walls;

    static int s_post_index;
    static std::unordered_map<int, size_t> s_ball_id_map;
    struct Ball {
        Circle circle;
        PhysicsBody body;
        Rect getBounds() const { return circle.getBounds(); }
    };
    static TypeContainer<Ball> s_balls;

    struct HurtBox {
        HurtboxType type;
        Circle circle;
        Rect getBounds() const { return circle.getBounds(); }
    };
    static TypeContainer<HurtBox> s_hurtboxes;

public:
    // functions called during initialization
    static void initialize(const Rect& bounds, size_t horizontal_subdivisions, size_t vertical_subdivisions);
    static void addWall(const Line& line);

    // functions called before every physics frame
    static void startFrame();

    // functions called during physics frames
    static void addBall(int id, const Circle& circle, const PhysicsBody& body);
    static void addHurtbox(HurtboxType type, const Circle& circle);
    static void addPost(const Circle& circle, const PhysicsBody& body); // uses ball interface, but does not clash with marble index

    // functions called during collision resolution
    static void forBallCollisions(int id, std::function<void(const PhysicsBody&, CollisionInfo)> callback);
    static void forPostCollisions(const Circle& previous, const Circle& current, std::function<void(CollisionInfo)> callback);
    static void forWallCollisions(const Circle& previous, const Circle& current, std::function<void(CollisionInfo)> callback);
    static bool hitsAny(HurtboxType type, const Circle& circle);
};

#endif // COLLISIONMANAGER_H
