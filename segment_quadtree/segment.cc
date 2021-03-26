#include "segment.hh"

#include <cmath>

namespace segment_quadtree {

namespace {

// Returns true if value is zero or close to zero.
bool is_zero(double value) {
    return std::abs(value) <= 1e-10;
}

// Returns true if a and b are both positive or both negative.
// Returns false if a or b are zero.
bool same_sign(double a, double b) {
    return (a > 0 and b > 0) or (a < 0 and b < 0);
}

} // namespace


Segment::Segment(const Point &a, const Point &b)
    : a_(a)
    , b_(b)
    , dx_(b.x_ - a.x_)
    , dy_(b.y_ - a.y_)
    , length_(std::sqrt(dx_ * dx_ + dy_ * dy_))
    , box_(a_, b_) {
    dx_ /= length_;
    dy_ /= length_;
}

bool Segment::connects(const Segment &other) const {
    return a_.is_coincident(other.a_)
        or a_.is_coincident(other.b_)
        or b_.is_coincident(other.a_)
        or b_.is_coincident(other.b_);
}

bool Segment::touches(const Segment &other) const {
    // First check if bounding boxes touch.
    if (not box_.touches(other.box_)) {
        return false;
    }

    // Check cross w.r.t. this segment.
    auto cross_a = cross(other.a_);
    auto cross_b = cross(other.b_);
    auto any_zero = is_zero(cross_a) or is_zero(cross_b);
    if (same_sign(cross_a, cross_b) and not any_zero) {
        return false;
    }

    // Check cross w.r.t. other segment.
    auto cross_a_other = other.cross(a_);
    auto cross_b_other = other.cross(b_);
    auto any_zero_other = is_zero(cross_a_other) or is_zero(cross_b_other);
    if (same_sign(cross_a_other, cross_b_other) and not any_zero_other) {
        return false;
    }

    return any_zero or any_zero_other;
}

bool Segment::intersects(const Segment &other) const {
    // First check if bounding boxes touch.
    if (not box_.touches(other.box_)) {
        return false;
    }

    // Check cross w.r.t. this segment.
    auto cross_a = cross(other.a_);
    auto cross_b = cross(other.b_);
    auto any_zero = is_zero(cross_a) or is_zero(cross_b);
    if (same_sign(cross_a, cross_b) and not any_zero) {
        return false;
    }

    // Check cross w.r.t. other segment.
    auto cross_a_other = other.cross(a_);
    auto cross_b_other = other.cross(b_);
    auto any_zero_other = is_zero(cross_a_other) or is_zero(cross_b_other);
    if (same_sign(cross_a_other, cross_b_other) and not any_zero_other) {
        return false;
    }

    return true;
}

bool Segment::intersects(const Box &box) const {
    if (box.touches(a_) or box.touches(b_)) {
        return true;
    }
    return intersects(Segment(Point{box.xmin_, box.ymin_}, Point{box.xmin_, box.ymax_}))
        or intersects(Segment(Point{box.xmin_, box.ymin_}, Point{box.xmax_, box.ymin_}))
        or intersects(Segment(Point{box.xmax_, box.ymin_}, Point{box.xmax_, box.ymax_}))
        or intersects(Segment(Point{box.xmin_, box.ymax_}, Point{box.xmax_, box.ymax_}));
}

double Segment::cross(const Point &point) const {
    auto dx = point.x_ - a_.x_;
    auto dy = point.y_ - a_.y_;
    auto d = std::sqrt(dx * dx + dy * dy);
    dx /= d;
    dy /= d;
    return dx * dy_ - dy * dx_;
}

} // namespace segment_quadtree
