#pragma once

#include <models/point.hpp>
#include <models/shape.hpp>


namespace mvc::shapes {

/// @brief Triangle shape.
class Triangle : public Shape {
public:
    /// @brief Constructor.
    /// @param fp first point
    /// @param sp second point
    /// @param tp third point
    Triangle(const Point& fp, const Point& sp, const Point& tp) {
        points_.push_back(fp);
        points_.push_back(sp);
        points_.push_back(tp);
        points_.push_back(fp);
    }
    virtual ~Triangle() {}

    /// @brief Returns string representation of the shape.
    /// @return 
    std::string GetInfo() const override {
        return "triangle";
    }
};

} // namespace mvc::shapes
