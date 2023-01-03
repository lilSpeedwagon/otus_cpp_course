#pragma once

#include <models/point.hpp>
#include <models/shape.hpp>


namespace mvc::shapes {

/// @brief Rectangle shape.
class Rectangle : public Shape {
public:
    /// @brief Constructor.
    /// @param lbc left bottom corner point
    /// @param rtc right top corner point
    Rectangle(const Point& lbc, const Point& rtc) {
        points_.push_back(lbc);
        points_.push_back({lbc.x, rtc.y});
        points_.push_back(rtc);
        points_.push_back({rtc.x, lbc.y});
        points_.push_back(lbc);
    }
    virtual ~Rectangle() {}

    std::string GetInfo() const override {
        return "rectangle";
    }
};

} // namespace mvc::shapes
