#include "box.hh"

namespace segment_quadtree {

Box::Box(const Point &a, const Point &b)
    : xmin_(std::min(a.x_, b.x_))
    , xmax_(std::max(a.x_, b.x_))
    , ymin_(std::min(a.y_, b.y_))
    , ymax_(std::max(a.y_, b.y_)) {
}

bool Box::touches(const Box& other) const {
    bool too_high   {ymin_ > other.ymax_};
    bool too_low    {ymax_ < other.ymin_};
    bool left       {xmax_ < other.xmin_};
    bool right      {xmin_ > other.xmax_};
    bool outside {too_high or too_low or left or right};
    return not outside;
}

bool Box::touches(const Point &point) const {
    return touches(point.x_, point.y_);
}

bool Box::touches(double x, double y) const {
    bool within_x = x >= xmin_ and x <= xmax_;
    bool within_y = y >= ymin_ and y <= ymax_;
    return within_x and within_y;
}

void Box::include(double x, double y) {
    xmin_ = std::min(x, xmin_);
    xmax_ = std::max(x, xmax_);
    ymin_ = std::min(y, ymin_);
    ymax_ = std::max(y, ymax_);
}

void Box::include(const Point &point) {
    include(point.x_, point.y_);
}

void Box::include(const Box &box) {
    include(box.xmin_, box.ymin_);
    include(box.xmin_, box.ymax_);
    include(box.xmax_, box.ymin_);
    include(box.xmax_, box.ymax_);
}

Point Box::center() const {
    return Point{(xmin_ + xmax_) / 2, (ymin_ + ymax_) / 2};
}


} // namespace segment_quadtree
