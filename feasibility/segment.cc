#include "segment.hh"

#include <cassert>
#include <utility>

namespace feasibility {

Segment Segment::split(const Cycle &cycle, point_id_t split_point, Direction removal_direction) {
    assert(direction(cycle) != Direction::None);

    // Get sequence numbers.
    const auto first_seq = cycle.sequence_[first_];
    const auto second_seq = cycle.sequence_[second_];
    const auto split_seq = cycle.sequence_[split_point];

    // Check sequence numbers.
    if (direction(cycle) == Direction::Forward) {
        assert(second_seq >= split_seq);
        if (second_seq == split_seq) {
            assert(removal_direction != Direction::Forward);
        }
        assert(split_seq >= first_seq);
        if (first_seq == split_seq) {
            assert(removal_direction != Direction::Backward);
        }
    } else {
        assert(second_seq <= split_seq);
        if (second_seq == split_seq) {
            assert(removal_direction != Direction::Forward);
        }
        assert(split_seq <= first_seq);
        if (first_seq == split_seq) {
            assert(removal_direction != Direction::Backward);
        }
    }

    // Get new endpoint.
    const auto new_endpoint = removal_direction == Direction::Forward
        ? cycle.next_[split_point]
        : cycle.prev_[split_point];

    // Update second point on this segment, perform split, and return new segment.
    auto original_second = second_;
    if (direction(cycle) == removal_direction) {
        second_ = split_point;
        return Segment(new_endpoint, original_second);
    } else {
        second_ = new_endpoint;
        return Segment(split_point, original_second);
    }
}

void Segment::reverse() {
    std::swap(first_, second_);
}

primitives::sequence_t Segment::first_sequence(const Cycle &cycle) const {
    return cycle.sequence_[first_];
}

primitives::sequence_t Segment::second_sequence(const Cycle &cycle) const {
    return cycle.sequence_[second_];
}

primitives::Direction Segment::direction(const Cycle &cycle) const {
    if (second_sequence(cycle) == first_sequence(cycle)) {
        return Direction::None;
    } else if (second_sequence(cycle) > first_sequence(cycle)) {
        return Direction::Forward;
    } else {
        return Direction::Backward;
    }
}

} // namespace feasibility

