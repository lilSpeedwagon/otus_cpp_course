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
    ShapesFactory() {
        Init();
    }
    ~ShapesFactory() {}

    /// @brief Builds Shape instance by its name and provided set of params.
    /// @param shape_name name of the shape to build
    /// @param params vector of float parameters required to build Shape
    /// @return shared_ptr to built Shape
    /// @throws std::logic_error is number of points doesn't match the provided shape_name
    /// @throws std::runtime_error if unknown shape name specified
    ShapePtr BuildShape(const std::string& shape_name,
                        const std::vector<float>& params) const {
        if (params.size() != GetParams(shape_name).size()) {
            throw std::logic_error("invalid params number");
        }

        ShapePtr result_ptr = nullptr;
        if (shape_name == "rect") {
            Point lbc{params[0], params[1]};
            Point rtc{params[2], params[3]};
            auto ptr = std::make_shared<Rectangle>(lbc, rtc);
            result_ptr = std::move(ptr);
        } else if (shape_name == "triangle") {
            Point fp{params[0], params[1]};
            Point sp{params[2], params[3]};
            Point tp{params[4], params[5]};
            auto ptr = std::make_shared<Triangle>(fp, sp, tp);
            result_ptr = std::move(ptr);
        } else if (shape_name == "circle") {
            Point center{params[0], params[1]};
            float radius = params[2];
            auto ptr = std::make_shared<Circle>(center, radius);
            result_ptr = std::move(ptr);
        }
        return result_ptr;
    }

    /// @brief Returns the vector of parameters required for building Shape.
    /// @param shape_name name of the Shape
    /// @return vector of required parameters
    /// @throws std::runtime_error if unknown shape name specified
    std::vector<std::string> GetParams(const std::string& shape_name) const {
        if (auto it = points_map_.find(shape_name);
            it != points_map_.end()) {
            return it->second;
        }
        throw std::runtime_error("unknown shape");
    }

private:
    void Init() {
        points_map_ = {
            {"rect", {"left bottom corner X",
                      "left bottom corner Y",
                      "right top corner X",
                      "right top corner Y"}},
            {"triangle", {"first point x",
                          "first point y",
                          "second point x",
                          "second point y",
                          "third point x",
                          "third point y"}},
            {"circle", {"center x",
                        "center y",
                        "radius"}}
        };
    }

    std::unordered_map<std::string, std::vector<std::string>> points_map_;
};

} // namespace mvc::shapes
