#pragma once

#include <memory>
#include <string>

#include <models/point.hpp>
#include <utils/typedef.hpp>


namespace mvc::shapes {

/// @brief Abstract vector shape.
class Shape {
public:
    virtual ~Shape() {}

    /// @brief Returns string representation of the shape.
    virtual std::string GetInfo() const = 0;
    
    /// @brief Returns points of the shape polygon.
    const Points& GetPoints() const {
        return points_;
    }

protected:
    Points points_;
    Shape() {}
};

using ShapePtr = std::shared_ptr<Shape>;
using ShapeId = utils::StrongTypedef<size_t, struct ShapeIdTag>;

} // namespace mvc::shapes
