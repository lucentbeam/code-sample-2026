#ifndef GRID_H
#define GRID_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <functional>

#include "utils/shapes.h"


// provides a basic interface for space partitioning
//  elements are inserted by ID & Rect
//  overlaps are determined by checking a subgrid and then returning Rect/Rect collisions
class CollisionGrid {
    std::vector<std::unordered_set<size_t>> m_grid;
    std::unordered_map<size_t, Rect> m_entry_bounds;

    Rect m_bounds;
    Vec2 m_cell_spacing;

    size_t m_subdivisions_wide;
    size_t m_subdivisions_tall;

    bool ready() const { return m_grid.size() > 0; }

    void forEachOverlap(Rect bounds, std::function<void(std::unordered_set<size_t>&)> callback);
    void forEachOverlapConst(Rect bounds, std::function<void(const std::unordered_set<size_t>&)> callback) const;
public:
    CollisionGrid() = default;

    void configure(Rect bounds, size_t horizontal_subdivisions, size_t vertical_subdivisions);

    void clear();

    void insert(size_t id, Rect bounds);

    std::unordered_set<size_t> getOverlaps(Rect bounds) const;

    std::unordered_set<size_t> getOverlaps(size_t id) const;
};

#endif // GRID_H
