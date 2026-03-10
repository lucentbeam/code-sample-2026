#include "grid.h"

#include <iostream>

void CollisionGrid::forEachOverlap(Rect rect, std::function<void (std::unordered_set<size_t>&)> callback)
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

    for (size_t y = rect.top_left.y; y < rect.bottom_right.y; ++y) {
        for (size_t x = rect.top_left.x; x < rect.bottom_right.x; ++x) {
            callback(m_grid[x + y * m_subdivisions_wide]);
        }
    }
}

void CollisionGrid::forEachOverlapConst(Rect rect, std::function<void (const std::unordered_set<size_t>&)> callback) const
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

    for (size_t y = rect.top_left.y; y < rect.bottom_right.y; ++y) {
        for (size_t x = rect.top_left.x; x < rect.bottom_right.x; ++x) {
            callback(m_grid[x + y * m_subdivisions_wide]);
        }
    }
}

void CollisionGrid::configure(Rect bounds, size_t horizontal_subdivisions, size_t vertical_subdivisions)
{
    m_bounds = bounds;
    m_subdivisions_wide = size_t(std::fmax(horizontal_subdivisions, 1));
    m_subdivisions_tall = size_t(std::fmax(vertical_subdivisions, 1));
    m_grid.clear();
    m_grid.resize(m_subdivisions_wide * m_subdivisions_tall);

    m_cell_spacing = m_bounds.span() / Vec2(m_subdivisions_wide, m_subdivisions_tall);
}

void CollisionGrid::clear() {
    m_grid.clear();
    m_grid.resize(m_subdivisions_wide * m_subdivisions_tall);
}

void CollisionGrid::insert(size_t id, Rect bounds)
{
    m_entry_bounds[id] = bounds;
    forEachOverlap(bounds, [&id](std::unordered_set<size_t>& set){
        set.insert(id);
    });
}

std::unordered_set<size_t> CollisionGrid::getOverlaps(Rect bounds) const
{
    std::unordered_set<size_t> result;
    forEachOverlapConst(bounds, [&bounds, &result, this](const std::unordered_set<size_t>& set){
        for (const int &id : set) {
            if (m_entry_bounds.at(id).overlaps(bounds)) {
                result.insert(id);
            }
        }
    });
    return result;
}

std::unordered_set<size_t> CollisionGrid::getOverlaps(size_t id) const
{
    auto it = m_entry_bounds.find(id);
    if (it != m_entry_bounds.end()) {
        return getOverlaps(it->second);
    }
    std::cout << "WARNING: CollisionGrid request for Rect ID " << id << " failed" << std::endl;
    return std::unordered_set<size_t>();
}
