#pragma once

#include <boost/math/constants/constants.hpp>

#include <models/point.hpp>
#include <models/shape.hpp>


namespace mvc::shapes {

/// @brief Circle shape.
class Circle : public Shape {
public:
    /// @brief Constructor.
    /// @param center circle center point
    /// @param radius radius of the circle
    Circle(const Point& center, float radius) {
        constexpr const auto angle_step = boost::math::constants::pi<float>() / 12.0f;
        constexpr const auto angle_max = 2.0f * boost::math::constants::pi<float>();
        float angle = 0.0f;
        while (angle < angle_max) {
            float x = center.x + static_cast<float>(std::cos(angle) * radius);
            float y = center.y + static_cast<float>(std::sin(angle) * radius); 
            Point p{x, y};
            points_.push_back(p);
            angle += angle_step;
        }
        points_.push_back(points_.front());
    }
    virtual ~Circle() {}

    std::string GetInfo() const override {
        return "circle";
    }
};

} // namespace mvc::shapes
