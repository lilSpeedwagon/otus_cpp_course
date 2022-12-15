#pragma once

#include "point.hpp"
#include "shape.hpp"

namespace mvc::shapes {

/// @brief Rectangle shape.
class Rectangle : public Shape {
public:
    /// @brief Constructor.
    /// @param lbc left bottom corner point
    /// @param rtc right top corner point
    Rectangle(const Point& lbc, const Point& rtc) {
        points_.push_back(lbc);
        points_.emplace_back(lbc.x, rtc.y);
        points_.push_back(rtc);
        points_.emplace_back(rtc.x, lbc.y);
        points_.push_back(lbc);
    }
    virtual ~Rectangle() {}
};

} // namespace mvc::shapes
