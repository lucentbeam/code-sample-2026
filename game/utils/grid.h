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

    std::vector<std::unordered_set<int>> m_grid;
    std::unordered_map<int, Rect> m_entry_bounds;

    Rect m_bounds;
    Vec2 m_cell_spacing;

    size_t m_subdivisions_wide;
    size_t m_subdivisions_tall;

    bool ready() const { return m_grid.size() > 0; }

    void forEachOverlap(Rect bounds, std::function<void(std::unordered_set<int>&)> callback);
public:
    CollisionGrid() = default;

    // Note: Subdivisions should be positive and integer. It would probably be nicer, for this,
    //       to support templated Vec2's (a la SFML) so that it could be a Vec2<unsigned int>
    //       parameter, but for brevity I'm leaving this as a simpler albeit more treacherous
    //       interface
    void configure(Rect bounds, Vec2 subdivisions);

    void clear();

    void insert(int id, Rect bounds);

    std::unordered_set<int> getOverlaps(Rect bounds);

    std::unordered_set<int> getOverlaps(int id);
};

#endif // GRID_H
