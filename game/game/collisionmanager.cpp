#include "collisionmanager.h"

CollisionManager::TypeContainer<Line> CollisionManager::s_walls;

int CollisionManager::s_post_index = -1;
std::unordered_map<int, size_t> CollisionManager::s_ball_id_map;
CollisionManager::TypeContainer<CollisionManager::Ball> CollisionManager::s_balls;

CollisionManager::TypeContainer<CollisionManager::HurtBox> CollisionManager::s_hurtboxes;

void CollisionManager::initialize(const Rect& bounds, size_t horizontal_subdivisions, size_t vertical_subdivisions)
{
    s_balls.grid.configure(bounds, horizontal_subdivisions, vertical_subdivisions);
    s_walls.grid.configure(bounds, horizontal_subdivisions, vertical_subdivisions);
    s_hurtboxes.grid.configure(bounds, horizontal_subdivisions, vertical_subdivisions);
}

void CollisionManager::addWall(const Line& line)
{
    s_walls.add(line);
}

void CollisionManager::startFrame()
{
    s_balls.clear();
    s_ball_id_map.clear();
    s_hurtboxes.clear();
    s_post_index = -1;
}

void CollisionManager::addBall(int id, const Circle& circle, const PhysicsBody& body)
{
    if (s_ball_id_map.find(id) != s_ball_id_map.end()) return;

    size_t internal_id = s_balls.entries.size();
    s_ball_id_map[id] = internal_id;

    s_balls.add(Ball{circle, body});
}

void CollisionManager::addHurtbox(HurtboxType type, const Circle& circle)
{
    s_hurtboxes.add(HurtBox{type, circle});
}

void CollisionManager::addPost(const Circle& circle, const PhysicsBody& body)
{
    addBall(s_post_index, circle, body);
    s_post_index--;
}

void CollisionManager::forBallCollisions(int id, std::function<void (const PhysicsBody&, CollisionInfo)> callback)
{
    auto it = s_ball_id_map.find(id);
    if (it == s_ball_id_map.end()) return;

    size_t internal_index = it->second;
    Circle source = s_balls.entries[internal_index].circle;

    for (size_t neighbor : s_balls.grid.getOverlaps(internal_index)) {
        if (neighbor == internal_index) continue;
        CollisionInfo info = CollisionDetection::circleCircle(source, s_balls.entries[neighbor].circle);
        if (info.collides) {
            callback(s_balls.entries[neighbor].body, info);
        }
    }
}

void CollisionManager::forWallCollisions(const Circle& previous, const Circle& current, std::function<void (CollisionInfo)> callback)
{
    Rect bounds = previous.getBounds().unionWith(current.getBounds());
    std::unordered_set<int> candidates = s_walls.grid.getOverlaps(bounds);

    for (int id : candidates) {
        const Line& wall = s_walls.entries[id];
        CollisionInfo info = CollisionDetection::circleLine(current, wall);
        // if the current frame has a collision or the circle phased across
        if (info.collides || (current.center - info.contact_point).dot(previous.center - info.contact_point) < 0) {
            callback(info);
        }
    }
}

bool CollisionManager::hitsAny(HurtboxType type, const Circle& circle)
{
    std::unordered_set<int> candidates = s_hurtboxes.grid.getOverlaps(circle.getBounds());

    for (int id : candidates) {
        const HurtBox& box = s_hurtboxes.entries[id];
        if (box.type == type && CollisionDetection::circleCircle(circle, box.circle).collides) {
            return true;
        }
    }
    return false;
}
