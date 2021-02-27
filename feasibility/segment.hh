#pragma once

#include <constants.hh>
#include <primitives.hh>
#include <vector>

namespace feasibility {

// Represents one contiguous portion of the tour, storing only the endpoints.
// Useful in keeping track of how a tour is split from successive edge removals.
class Segment {
    using point_id_t = primitives::point_id_t;
    using Direction = primitives::Direction;
    template<typename T>
    using vector = std::vector<T>;

    Segment(point_id_t first, point_id_t second) : first_(first), second_(second) {}

    // Splits this segment given a point and edge direction.
    // Modifies this segment to be the first segment and returns the second segment.
    Segment split(const vector<point_id_t> &sequence, point_id_t split_point, Direction removal_direction);

    point_id_t first_{constants::INVALID_POINT};
    point_id_t second_{constants::INVALID_POINT};
};

} // namespace feasibility

