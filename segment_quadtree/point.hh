#pragma once


namespace segment_quadtree {

struct Point {
    double x_{0};
    double y_{0};

    double distance(const Point &other) const;

    // returns true if points are the same (distance is 0).
    bool is_coincident(const Point &other) const;
};

} // namespace segment_quadtree
