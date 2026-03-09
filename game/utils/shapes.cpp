#include "shapes.h"

bool Rect::overlaps(const Rect& other) const
{
    return left() <= other.right()
            && right() >= other.left()
            && top() <= other.bottom()
            && bottom() >= other.top();
}

Rect Rect::intersectionWith(const Rect& other) const {
    Vec2 tl = Vec2(std::fmax(top_left.x, other.top_left.x), std::fmax(top_left.y, other.top_left.y));
    Vec2 br = Vec2(std::fmin(bottom_right.x, other.bottom_right.x), std::fmin(bottom_right.y, other.bottom_right.y));
    return Rect{tl, br};
}

Rect Rect::unionWith(const Rect& other) const
{
    Vec2 tl = Vec2(std::fmin(top_left.x, other.top_left.x), std::fmin(top_left.y, other.top_left.y));
    Vec2 br = Vec2(std::fmax(bottom_right.x, other.bottom_right.x), std::fmax(bottom_right.y, other.bottom_right.y));
    return Rect{tl, br};
}

Circle::Circle(Vec2 center, double radius) :
    center(center),
    radius(radius)
{

}

Circle::Circle(double radius) :
    center(),
    radius(radius)
{

}

Rect Circle::getBounds() const
{
    Vec2 offset = Vec2(1,1) * radius;
    return {center - offset, center + offset};
}

Line::Line(Vec2 start, Vec2 end) : start(start), dir(end - start), length(dir.length())
{
    if (length > 0) dir /= length;
}

Vec2 Line::normal() const { return dir.right(); }

Rect Line::getBounds() const
{
    double x = std::fmin(start.x, start.x + dir.x * length);
    double y = std::fmin(start.y, start.y + dir.y * length);
    double w = std::fabs(dir.x * length);
    double h = std::fabs(dir.y * length);

    return {Vec2(x, y), Vec2(x + w, y + h)};
}

