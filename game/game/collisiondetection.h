// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "core/utility.h"

#include "utils/shapes.h"

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

class CollisionDetection
{
public:
    static CollisionInfo circleLine(const Circle& circle, const Line& line);

    static CollisionInfo circleCircle(const Circle& circle1, const Circle& circle2);

    static bool overlapsRect(const Circle&, const Rect&);
};

#endif // COLLISIONDETECTION_H
