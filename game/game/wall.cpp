#include "wall.h"

#include <cmath>

Wall::Wall(Vec2 start, Vec2 end) :
    m_start(start),
    m_end(end),
    m_dir((end - start).normalized()),
    m_length((end-start).length())
{

}

WallNearestLoc Wall::getNearest(const Vec2 &point) const
{
    // find the distance along the infinite wall to the nearest point
    double closest_dist = (point - m_start).dot(m_dir);

    // clamp to the start & end of the wall
    closest_dist = std::fmin(std::fmax(closest_dist, 0), m_length);

    WallNearestLoc result;
    result.wall_location = m_start + m_dir * closest_dist;
    result.wall_normal = m_dir.right();

    result.wall_location_to_point = point - result.wall_location;
    result.distance = result.wall_location_to_point.length();

    return result;
}
