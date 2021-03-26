#pragma once

namespace segment_quadtree {
namespace constants {

// Splits quadtree node if number of segments contained exceeds this number.
constexpr int SPLIT_THRESHOLD{2};

// No quadtree nodes will be created beyond this depth.
constexpr int MAX_DEPTH{15};

} // namespace constants
} // namespace segment_quadtree
