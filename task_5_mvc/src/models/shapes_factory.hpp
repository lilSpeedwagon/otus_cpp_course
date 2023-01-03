#pragma once

#include <string>
#include <vector>

#include <models/circle.hpp>
#include <models/shape.hpp>
#include <models/rectangle.hpp>
#include <models/triangle.hpp>


namespace mvc::shapes {

/// @brief Factory class for constructing instances of Shape.
class ShapesFactory {
public:
    ShapesFactory();
    ~ShapesFactory();

    /// @brief Builds Shape instance by its name and provided set of params.
    /// @param shape_name name of the shape to build
    /// @param params vector of float parameters required to build Shape
    /// @return shared_ptr to built Shape
    /// @throws std::logic_error is number of points doesn't match the provided shape_name
    /// @throws std::runtime_error if unknown shape name specified
    ShapePtr BuildShape(const std::string& shape_name,
                        const std::vector<float>& params) const;

    /// @brief Returns the vector of parameters required for building Shape.
    /// @param shape_name name of the Shape
    /// @return vector of required parameters
    /// @throws std::runtime_error if unknown shape name specified
    std::vector<std::string> GetParams(const std::string& shape_name) const;

private:
    void Init();

    std::unordered_map<std::string, std::vector<std::string>> points_map_;
};

} // namespace mvc::shapes
