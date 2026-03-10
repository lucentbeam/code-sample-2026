// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#ifndef SHAPES_H
#define SHAPES_H

#include <cmath>

#include "core/utility.h"

struct Rect {
    Vec2 top_left;
    Vec2 bottom_right;

    double left() const { return top_left.x; }
    double right() const { return bottom_right.x; }
    double top() const { return top_left.y; }
    double bottom() const { return bottom_right.y; }

    bool overlaps(const Rect& other) const;

    // the rect that intersects both rects. other is outside of this rect, this gets clamped within other's edges
    Rect intersectionWith(const Rect& other) const;

    // the larger rect that encompasses both
    Rect unionWith(const Rect& other) const;

    Vec2 span() const { return bottom_right - top_left; }
};

class Collider {
public:
    virtual ~Collider() {}

    virtual Rect getBounds() const = 0;

    enum Type { Circle, Line };
    virtual Type getType() const = 0;
};

struct Circle : public Collider {
    Vec2 center;
    double radius;

    Circle() = default;
    Circle(double radius);
    Circle(Vec2 center, double radius);

    // Collider interface
    Rect getBounds() const override;
    Collider::Type getType() const override { return Collider::Circle; }
};

// Lines are one-sided and should be constructed with their normal on their right
struct Line : public Collider {
    Vec2 start;
    Vec2 dir;
    double length;

    Line(Vec2 start, Vec2 end);
    Vec2 normal() const;

    // Collider interface
    Rect getBounds() const override;
    Collider::Type getType() const override { return Collider::Line; }
};

#endif // SHAPES_H
