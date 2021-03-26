#include "segment_quadtree.hh"
#include "constants.hh"

#include <algorithm>

namespace segment_quadtree {

namespace {

Box get_box(const std::vector<Segment> &segments) {
    double xmin{segments[0].a_.x_};
    double ymin{segments[0].a_.y_};
    double xmax{segments[0].a_.x_};
    double ymax{segments[0].a_.y_};
    for (const auto &s : segments) {
        for (const auto &p : {s.a_, s.b_}) {
            xmin = std::min(xmin, p.x_);
            ymin = std::min(ymin, p.y_);
            xmax = std::max(xmax, p.x_);
            ymax = std::max(ymax, p.y_);
        }
    }
    constexpr double FUDGE{1e-6}; // small padding to ensure all point fall within root node box.
    return Box({xmin - FUDGE, ymin - FUDGE}, {xmax + FUDGE, ymax + FUDGE});
}

} // namespace

SegmentQuadtree::SegmentQuadtree(const std::vector<Segment> &segments)
    : segments_(segments)
    , root_(get_box(segments)) {
    for (size_t i{0}; i < segments_.size(); ++i) {
        root_.insert(segments_, i);
    }
}

void SegmentQuadtree::validate() {
    max_residency_ = 0;
    avg_residency_ = 0;
    max_depth_ = 0;
    leaves_ = 0;
    seen_segment_ids_.clear();
    node_count_ = 0;
    validate(root_);
    assert(seen_segment_ids_.size() == segments_.size());
    std::cout << "node count: " << node_count_ << std::endl;
    std::cout << "node count ratio: " << double(node_count_) / segments_.size() << std::endl;
    std::cout << "max depth: " << max_depth_ << std::endl;
    std::cout << "average residency: " << avg_residency_ / double(leaves_) << std::endl;
    std::cout << "max residency: " << max_residency_ << std::endl;
    seen_segment_ids_.clear();
}

void SegmentQuadtree::validate(const SegmentNode &node) {
    ++node_count_;
    if (node.is_leaf()) {
        assert(not node.segments_.empty());
        for (auto id : node.segments_) {
            seen_segment_ids_.insert(id);
        }
        ++leaves_;
        avg_residency_ += int(node.segments_.size());
        max_residency_ = std::max(max_residency_, int(node.segments_.size()));
        max_depth_ = std::max(max_depth_, node.depth_);
        if (node.depth_ + 1 < constants::MAX_DEPTH) {
            assert(node.segments_.size() <= constants::SPLIT_THRESHOLD);
        }
    } else {
        assert(node.segments_.empty());
    }
    for (const auto &child : node.children_) {
        if (child) {
            validate(*child);
        }
    }
}

bool SegmentQuadtree::intersects(const Segment &segment) const {
    return intersects(segment, root_);
}

bool SegmentQuadtree::intersects(const Segment &segment, const SegmentNode &current_node) const {
    if (current_node.is_leaf()) {
        for (const auto &i : current_node.segments_) {
            if (segment.intersects(segments_[i])) {
                if (not segment.connects(segments_[i])) { // TODO: look into the case where segments are co-linear.
                    return true;
                }
            }
        }
    } else {
        for (const auto &child : current_node.children_) {
            if (child) {
                if (segment.intersects(child->box_)) {
                    if (intersects(segment, *child)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

} // namespace segment_quadtree
