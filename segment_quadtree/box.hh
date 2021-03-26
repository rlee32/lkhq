#pragma once

#include "point.hh"

#include <iostream>
#include <algorithm>
#include <limits>

namespace segment_quadtree {

struct Box {
    Box() = default;
    // Constructs a box from any 2 diagonally-opposite corners.
    Box(const Point &a, const Point &b);

    double xmin_ {std::numeric_limits<double>::max()};
    double xmax_ {std::numeric_limits<double>::min()};
    double ymin_ {std::numeric_limits<double>::max()};
    double ymax_ {std::numeric_limits<double>::min()};

    bool touches(const Box& other) const;
    bool touches(const Point &point) const;
    bool touches(double x, double y) const;

    void include(double x, double y);
    // if x, y is not in this box, the box is enlarged to contain it.
    void include(const Point &point);
    // this is enlarged to contain box.
    void include(const Box &box);

    Point center() const;
};

inline auto &operator<<(std::ostream &o, const Box &b) {
    o << "(" << b.xmin_ << ", " << b.ymin_ << ") "
        << "(" << b.xmax_ << ", " << b.ymax_ << ")" << std::endl;
    return o;
}

} // namespace segment_quadtree

