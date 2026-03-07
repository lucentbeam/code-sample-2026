#ifndef WALL_H
#define WALL_H

#include "core/utility.h"

struct WallNearestLoc {
    Vec2 wall_location;
    Vec2 wall_normal;

    Vec2 wall_location_to_point;
    double distance;
};

// walls should be constructed so that their right-hand side is their normal
class Wall
{
    Vec2 m_start, m_end;
    Vec2 m_dir;
    double m_length;
public:
    Wall(Vec2 start, Vec2 end);

    WallNearestLoc getNearest(const Vec2 &point) const;
};

#endif // WALL_H
