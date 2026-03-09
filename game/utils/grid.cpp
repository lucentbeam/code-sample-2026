#include "grid.h"

#include <iostream>

void CollisionGrid::forEachOverlap(Rect rect, std::function<void (std::unordered_set<int>&)> callback)
{
    if (!ready()) return;
    rect = rect.intersectionWith(m_bounds);

    rect.top_left -= m_bounds.top_left;
    rect.bottom_right -= m_bounds.top_left;

    rect.top_left /= m_cell_spacing;
    rect.bottom_right /= m_cell_spacing;

    // include right/bottom boundaries that pass solidly through cells,
    // but ignore ones exactly on the cell boundary, as in the case when
    // the rect gets clamped on the right and/or bottom sides
    rect.bottom_right.x -= std::numeric_limits<double>::epsilon();
    rect.bottom_right.y -= std::numeric_limits<double>::epsilon();

    for (int y = rect.top_left.y; y < rect.bottom_right.y; ++y) {
        for (int x = rect.top_left.x; x < rect.bottom_right.x; ++x) {
            callback(m_grid[x + y * m_subdivisions_wide]);
        }
    }
}

void CollisionGrid::configure(Rect bounds, Vec2 subdivisions)
{
    m_bounds = bounds;
    m_subdivisions_wide = size_t(std::fmax(subdivisions.x, 1));
    m_subdivisions_tall = size_t(std::fmax(subdivisions.y, 1));
    m_grid.clear();
    m_grid.resize(m_subdivisions_wide * m_subdivisions_tall);

    m_cell_spacing = m_bounds.span() / Vec2(m_subdivisions_wide, m_subdivisions_tall);
}

void CollisionGrid::clear() {
    m_grid.clear();
    m_grid.resize(m_subdivisions_wide * m_subdivisions_tall);
}

void CollisionGrid::insert(int id, Rect bounds)
{
    m_entry_bounds[id] = bounds;
    forEachOverlap(bounds, [&id](std::unordered_set<int>& set){
        set.insert(id);
    });
}

std::unordered_set<int> CollisionGrid::getOverlaps(Rect bounds)
{
    std::unordered_set<int> result;
    forEachOverlap(bounds, [&bounds, &result, this](std::unordered_set<int>& set){
        for (const int &id : set) {
            if (m_entry_bounds.at(id).overlaps(bounds)) {
                result.insert(id);
            }
        }
    });
    return result;
}

std::unordered_set<int> CollisionGrid::getOverlaps(int id)
{
    auto it = m_entry_bounds.find(id);
    if (it != m_entry_bounds.end()) {
        return getOverlaps(it->second);
    }
    std::cout << "WARNING: CollisionGrid request for Rect ID " << id << " failed" << std::endl;
    return std::unordered_set<int>();
}
