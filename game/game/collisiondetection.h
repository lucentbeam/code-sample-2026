#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "core/utility.h"

/*  Notes:
 *   These collision detection algorithms are highly simplified for the scope of this sample.
 *   Notably, contact_point cannot be determined from instantaneous positions;
 *   the contact point is a dynamical property that should be interpolated based on
 *   the current and previous frames.
 *
 *   For brevity, rectangles have also been left out, in favor of a simple boolean test.
 */

struct CollisionInfo {
    Vec2 contact_point;
    Vec2 normal;
    double distance;
    bool collides = false;
};

struct Circle {
    Vec2 center;
    double radius;
};

// Lines are one-sided and should be contstructed with their normal on their right
struct Line {
    Vec2 start;
    Vec2 dir;
    double length;

    Line(Vec2 start, Vec2 end);
    Vec2 normal() const { return dir.right(); }
};

class CollisionDetection
{
public:
    static CollisionInfo circleLine(const Circle& circle, const Line& line);

    static CollisionInfo circleCircle(const Circle& circle1, const Circle& circle2);

    static bool overlapsRect(const Circle&, const Vec2 top_left, const Vec2& bottom_right);
};

#endif // COLLISIONDETECTION_H
