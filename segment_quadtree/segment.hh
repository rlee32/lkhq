#pragma once

#include "point.hh"
#include "box.hh"

#include <iostream>

namespace segment_quadtree {

struct Segment {
public:
    // Constructs a segment from 2 points.
    Segment(const Point &a, const Point &b);

    // Returns true if this segment intersects with another segment.
    // If the segment is co-linear and overlapping, also returns true.
    // If the segment shares an endpoint but is not co-linear, it is also considered to be intersecting.
    bool intersects(const Segment &other) const;

    // Returns true if this segment 'touches' another segment.
    // This is a special case of intersection, where the endpoint of one segment lies along the other segment.
    bool touches(const Segment &other) const;

    // Returns true if this segment intersects with the provided box, or
    // if the segment is inside the box.
    bool intersects(const Box &box) const;

    auto length() const { return length_; }

    Point a_;
    Point b_;

    // x and y direction components (unit vector) from a to b.
    double dx_{0};
    double dy_{0};

    double length_{0};

    Box box_;

    // Performs a 2D cross product with the provided point (w.r.t. to a_) and b_ (w.r.t. to a_).
    double cross(const Point &point) const;

    // Returns true if this segment and other shares an endpoint.
    bool connects(const Segment &other) const;
};

inline auto &operator<<(std::ostream &o, const Segment &s) {
    o << "(" << s.a_.x_ << ", " << s.a_.y_ << ") "
        << "(" << s.b_.x_ << ", " << s.b_.y_ << ")" << std::endl;
    return o;
}

} // namespace segment_quadtree

