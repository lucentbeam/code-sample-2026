#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <vector>
#include <functional>

#include "collisiondetection.h"
#include "utils/shapes.h"
#include "utils/grid.h"
#include "game/physicsbody.h"

// Holds statics (stored once) and dynamics (stored per-frame) for checking collider overlaps
class CollisionManager
{
public:
    // functions called during initialization
    static void initialize(const Rect& bounds, const Vec2& subdivisions);
    static void addWall(const Line& line);

    // functions called during physics frames
    static void addBall(int id, const Circle& circle, const PhysicsBody& body);

    // functions called before every physics frame
    static void clearBalls();

    // functions called during collision resolution
    static void forBallCollisions(int id, std::function<void(const PhysicsBody&, CollisionInfo)> callback);
    static void forWallCollisions(const Circle& previous, const Circle& current, std::function<void(CollisionInfo)> callback);

private:
    static std::vector<Line> s_walls;
    static CollisionGrid s_wall_grid;

    static std::vector<Circle> s_balls;
    static std::vector<PhysicsBody> s_ball_bodies;
    static std::unordered_map<int, size_t> s_ball_id_map;
    static CollisionGrid s_ball_grid;
};

#endif // COLLISIONMANAGER_H
