#include "point.hh"

#include <cmath>

namespace segment_quadtree {

namespace {

// Returns true if value is zero or close to zero.
bool is_zero(double value) {
    return std::abs(value) <= 1e-7;
}

} // namespace


double Point::distance(const Point &other) const {
    auto dx = other.x_ - x_;
    auto dy = other.y_ - y_;
    return std::sqrt(dx * dx + dy * dy);
}

bool Point::is_coincident(const Point &other) const {
    auto dx = other.x_ - x_;
    auto dy = other.y_ - y_;
    return is_zero(dx) and is_zero(dy);
}

} // namespace segment_quadtree
