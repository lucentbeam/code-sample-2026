#include "collisiondetection.h"

#include <cmath>

Line::Line(Vec2 start, Vec2 end) : start(start), dir(end - start), length(dir.length())
{
    if (length > 0) dir /= length;
}

CollisionInfo CollisionDetection::circleLine(const Circle& circle, const Line& line)
{
    // find the distance along the infinite line to the nearest point
    double closest_dist = (circle.center - line.start).dot(line.dir);

    // clamp to the start & end of the line
    closest_dist = std::fmin(std::fmax(closest_dist, 0), line.length);

    CollisionInfo info;

    info.contact_point = line.start + line.dir * closest_dist;
    info.normal = line.normal();

    Vec2 to_point = circle.center - info.contact_point;

    // align the normal in the direction of the point
    if (info.normal.dot(to_point) < 0) info.normal *= -1;

    info.distance = to_point.length();
    info.collides = info.distance < circle.radius;

    return info;
}

CollisionInfo CollisionDetection::circleCircle(const Circle& circle1, const Circle& circle2)
{
    CollisionInfo info;

    info.normal = circle1.center - circle2.center;

    info.distance = info.normal.length();
    info.collides = info.distance < (circle1.radius + circle2.radius);

    if (info.distance > 0) info.normal /= info.distance; // normalize it
    else info.normal = Vec2(1, 0);
    info.contact_point = circle2.center + info.normal * circle2.radius;

    return info;
}

bool CollisionDetection::overlapsRect(const Circle& circle, const Rect& rect)
{
    // modeled after https://jeffreythompson.org/collision-detection/circle-rect.php

    Vec2 nearest_edge = circle.center;

    // temporary variable used to determine if circle.radius is needed
    bool outside = false;

    if (circle.center.x < rect.top_left.x) {
        // left edge
        nearest_edge.x = rect.top_left.x;
        outside = true;
    } else if (circle.center.x > rect.bottom_right.x) {
        // right edge
        nearest_edge.x = rect.bottom_right.x;
        outside = true;
    }

    if (circle.center.y < rect.top_left.y) {
        // top edge
        nearest_edge.y = rect.top_left.y;
        outside = true;
    } else if (circle.center.y > rect.bottom_right.y) {
        // bottom edge
        nearest_edge.y = rect.bottom_right.y;
        outside = true;
    }

    // circle center is inside the boundaries
    if (!outside) return true;

    // get distance from closest edges
    return (circle.center - nearest_edge).length() <= circle.radius;
}
