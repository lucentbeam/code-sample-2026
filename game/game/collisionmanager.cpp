// Copyright (c) 2026 Benjamin Hanken. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
#include "collisionmanager.h"

#include <iostream>

CollisionManager::CollisionObjectContainer CollisionManager::s_statics;

CollisionManager::CollisionObjectContainer CollisionManager::s_dynamics;

void CollisionManager::forCollisions(const CollisionObjectContainer& container, const CollisionObject& obj, int mask, std::function<void (const CollisionObject&, CollisionInfo)> callback)
{
    if (obj.collider->getType() != Collider::Circle) {
        std::cout << "WARNING: Only circle vs. other collisions supported right now " << std::endl;
        return;
    }
    std::unordered_set<size_t> candidates = container.grid.getOverlaps(obj.collider->getBounds());

    for (size_t id : candidates) {
        const CollisionObject& against = container.entries[id];
        if (against.collider == obj.collider) continue;
        if ((against.type & mask) == 0) continue;

        CollisionInfo info;
        switch(against.collider->getType()) {
        case Collider::Line:
            info = CollisionDetection::circleLine(*static_cast<Circle*>(obj.collider), *static_cast<Line*>(against.collider));
            if (!info.collides) {
                // special case for when a circle crosses the line in the last frame but is not within range
                Vec2 prev = obj.body->getPrevious() - info.contact_point;
                Vec2 curr = obj.body->getPosition() - info.contact_point;
                if (curr.dot(info.normal) * prev.dot(info.normal) < 0) {
                    info.collides = true;
                }
            }
            break;
        case Collider::Circle:
            info = CollisionDetection::circleCircle(*static_cast<Circle*>(obj.collider), *static_cast<Circle*>(against.collider));
            break;
        }

        if (info.collides) {
            callback(against, info);
        }
    }
}

void CollisionManager::initialize(const Rect& bounds, size_t horizontal_subdivisions, size_t vertical_subdivisions)
{
    s_statics.entries.clear();
    s_statics.grid.configure(bounds, horizontal_subdivisions, vertical_subdivisions);

    s_dynamics.entries.clear();
    s_dynamics.grid.configure(bounds, horizontal_subdivisions, vertical_subdivisions);
}

void CollisionManager::addStatic(CollisionObject obj)
{
    s_statics.add(obj);
}

void CollisionManager::startFrame()
{
    s_dynamics.clear();
}

void CollisionManager::addDynamic(CollisionObject obj)
{
    s_dynamics.add(obj);
}

bool CollisionManager::ifAnyCollision(Collider* collider, int mask)
{
    if (collider->getType() != Collider::Circle) {
        std::cout << "WARNING: Only circle vs. other collisions supported right now " << std::endl;
        return false;
    }
    std::unordered_set<size_t> candidates = s_statics.grid.getOverlaps(collider->getBounds());

    for (size_t id : candidates) {
        const CollisionObject& obj = s_statics.entries[id];
        if (obj.collider == collider) continue;
        if ((obj.type & mask) == 0) continue;

        CollisionInfo info;
        switch(obj.collider->getType()) {
        case Collider::Line:
            info = CollisionDetection::circleLine(*static_cast<Circle*>(collider), *static_cast<Line*>(obj.collider));
            break;
        case Collider::Circle:
            info = CollisionDetection::circleCircle(*static_cast<Circle*>(collider), *static_cast<Circle*>(obj.collider));
            break;
        }

        if (info.collides) {
            return true;
        }
    }

    candidates = s_dynamics.grid.getOverlaps(collider->getBounds());

    for (size_t id : candidates) {
        const CollisionObject& obj = s_dynamics.entries[id];
        if (obj.collider == collider) continue;
        if ((obj.type & mask) == 0) continue;

        CollisionInfo info;
        switch(obj.collider->getType()) {
        case Collider::Line:
            info = CollisionDetection::circleLine(*static_cast<Circle*>(collider), *static_cast<Line*>(obj.collider));
            break;
        case Collider::Circle:
            info = CollisionDetection::circleCircle(*static_cast<Circle*>(collider), *static_cast<Circle*>(obj.collider));
            break;
        }

        if (info.collides) {
            return true;
        }
    }

    return false;
}

void CollisionManager::forStaticCollisions(const CollisionObject& obj, int mask, std::function<void (const CollisionObject&, CollisionInfo)> callback)
{
    forCollisions(s_statics, obj, mask, callback);
}

void CollisionManager::forDynamicCollisions(const CollisionObject& obj, int mask, std::function<void (const CollisionObject&, CollisionInfo)> callback)
{
    forCollisions(s_dynamics, obj, mask, callback);
}
