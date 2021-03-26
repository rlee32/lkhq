#include "segment_node.hh"

#include "constants.hh"

namespace segment_quadtree {

SegmentNode::SegmentNode(const Box &box, int depth)
    : box_(box)
    , depth_(depth) {
}

void SegmentNode::create_child(const std::vector<Segment> &all_segments, int quadrant, const Box &box, const std::vector<int> &segments) {
    assert(not children_[quadrant]);
    children_[quadrant] = std::make_unique<SegmentNode>(box, depth_ + 1);
    children_[quadrant]->segments_ = segments;
    if (segments.size() > constants::SPLIT_THRESHOLD) {
        children_[quadrant]->split(all_segments);
    }
}

void SegmentNode::split(const std::vector<Segment> &all_segments) {
    assert(is_leaf());
    if (depth_ + 1 >= constants::MAX_DEPTH) {
        return;
    }
    for (int quadrant{0}; quadrant < 4; ++quadrant) {
        auto b = make_box(quadrant);
        std::vector<int> segments;
        for (const auto &i : segments_) {
            const auto &s = all_segments[i];
            if (s.intersects(b)) {
                segments.push_back(i);
            }
        }
        if (not segments.empty()) {
            create_child(all_segments, quadrant, b, segments);
        }
    }
    segments_.clear();
}

void SegmentNode::insert(const std::vector<Segment> &all_segments, int segment_id) {
    const auto &s = all_segments[segment_id];
    assert(s.intersects(box_));

    // max depth, insert into this node and return.
    if (depth_ + 1 >= constants::MAX_DEPTH) {
        segments_.push_back(segment_id);
        assert(is_leaf());
        return;
    }

    if (is_leaf()) {
        segments_.push_back(segment_id);
        if (segments_.size() > constants::SPLIT_THRESHOLD) {
            // Since we have too many segments in this node, split it up.
            split(all_segments);
        }
    } else {
        assert(segments_.empty());
        for (int quadrant{0}; quadrant < 4; ++quadrant) {
            auto &child = children_[quadrant];
            if (child) {
                if (s.intersects(child->box_)) {
                    child->insert(all_segments, segment_id);
                }
            } else {
                auto box = make_box(quadrant);
                create_child(all_segments, quadrant, box, {segment_id});
            }
        }
    }
}

bool SegmentNode::is_leaf() const {
    for (const auto &child : children_) {
        if (child) {
            return false;
        }
    }
    return true;
}

Box SegmentNode::make_box(int quadrant) const {
    auto center = box_.center();
    switch(quadrant) {
        case 0: return Box(center, {box_.xmax_, box_.ymax_});
        case 1: return Box(center, {box_.xmin_, box_.ymax_});
        case 2: return Box(center, {box_.xmin_, box_.ymin_});
        case 3: return Box(center, {box_.xmax_, box_.ymin_});
        default: {
            assert(quadrant < 4 and quadrant >= 0);
            return {};
        }
    }
}

} // namespace segment_quadtree
