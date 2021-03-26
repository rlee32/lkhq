#pragma once

#include "segment.hh"
#include "box.hh"

#include <algorithm>
#include <array>
#include <memory>
#include <vector>

namespace segment_quadtree {

struct SegmentNode {
    // Box is the area the segment node covers. depth is 0 for the root.
    SegmentNode(const Box &box, int depth = 0);

    // Inserts the segment identified by segment_id into this node, and creates children as necessary.
    // It is assumed that the segment touches this node.
    void insert(const std::vector<Segment> &all_segments, int segment_id);

    // Returns true if this node is a leaf node.
    bool is_leaf() const;

    // Children are ordered ccw by quadrant:
    // 0: +x, +y
    // 1: -x, +y
    // 2: -x, -y
    // 3: +x, -y
    std::array<std::unique_ptr<SegmentNode>, 4> children_;

    // IDs of segments in this node. Only leaf nodes may hold segments.
    std::vector<int> segments_;

    // Box representing physical coverage of this node.
    const Box box_;

    // Depth in the quadtree.
    int depth_{0};

    // Splits a leaf node into quadrants recursively until split threshold is satisfied or max depth is reached.
    void split(const std::vector<Segment> &all_segments);

    // Creates a child quadrant.
    void create_child(const std::vector<Segment> &all_segments, int quadrant, const Box &box, const std::vector<int> &segments);

    // Creates a box corresponding to quadrant.
    Box make_box(int quadrant) const;
};

} // namespace segment_quadtree
