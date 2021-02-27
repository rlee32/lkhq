#include "segment.hh"

#include <cassert>

namespace feasibility {

Segment Segment::split(const vector<point_id_t> &sequence, point_id_t split_point, Direction removal_direction) {
    const auto first_seq = sequence[first_];
    const auto second_seq = sequence[second_];
    const auto split_seq = sequence[split_point];
    assert(second_seq >= split_seq);
    if (second_seq == split_seq) {
        assert(removal_direction != Direction::Forward);
    }
    assert(split_seq >= first_seq);
    if (first_seq == split_seq) {
        assert(removal_direction != Direction::Backward);
    }
    // TODO: implement split (pending Cycle class).
    return {first_, second_};
}

} // namespace feasibility

