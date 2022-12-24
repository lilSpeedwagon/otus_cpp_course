#pragma once

#include <vector>


namespace mvc::shapes {

/// @brief A point in two-dimensional space.
struct Point {
    float x, y;
};

using Points = std::vector<Point>;

} // namespace mvc::shapes
