#include "collisionmanager.h"

std::vector<Line> CollisionManager::s_walls;

CollisionGrid CollisionManager::s_wall_grid;

std::vector<Circle> CollisionManager::s_balls;

std::vector<PhysicsBody> CollisionManager::s_ball_bodies;

std::unordered_map<int, size_t> CollisionManager::s_ball_id_map;

CollisionGrid CollisionManager::s_ball_grid;

void CollisionManager::initialize(const Rect& bounds, const Vec2& subdivisions)
{
    s_ball_grid.configure(bounds, subdivisions);
    s_wall_grid.configure(bounds, subdivisions);
}

void CollisionManager::addWall(const Line& line)
{
    size_t id = s_walls.size();
    s_walls.push_back(line);
    s_wall_grid.insert(int(id), line.getBounds());
}

void CollisionManager::addBall(int id, const Circle& circle, const PhysicsBody& body)
{
    if (s_ball_id_map.find(id) != s_ball_id_map.end()) return;

    size_t internal_id = s_balls.size();
    s_ball_id_map[id] = internal_id;
    s_ball_grid.insert(int(internal_id), circle.getBounds());
    s_balls.push_back(circle);
    s_ball_bodies.push_back(body);
}

void CollisionManager::clearBalls()
{
    s_balls.clear();
    s_ball_grid.clear();
    s_ball_id_map.clear();
    s_ball_bodies.clear();
}

void CollisionManager::forBallCollisions(int id, std::function<void (const PhysicsBody&, CollisionInfo)> callback)
{
    auto it = s_ball_id_map.find(id);
    if (it == s_ball_id_map.end()) return;

    size_t internal_index = it->second;
    Circle source = s_balls[internal_index];

    for (size_t neighbor : s_ball_grid.getOverlaps(internal_index)) {
        if (neighbor == internal_index) continue;
        CollisionInfo info = CollisionDetection::circleCircle(source, s_balls[neighbor]);
        if (info.collides) {
            callback(s_ball_bodies[neighbor], info);
        }
    }
}

void CollisionManager::forWallCollisions(const Circle& previous, const Circle& current, std::function<void (CollisionInfo)> callback)
{
    Rect bounds = previous.getBounds().unionWith(current.getBounds());
    std::unordered_set<int> candidates = s_wall_grid.getOverlaps(bounds);

    for (int id : candidates) {
        const Line& wall = s_walls[id];
        CollisionInfo info = CollisionDetection::circleLine(current, wall);
        // if the current frame has a collision or the circle phased across
        if (info.collides || (current.center - info.contact_point).dot(previous.center - info.contact_point) < 0) {
            callback(info);
        }
    }
}
