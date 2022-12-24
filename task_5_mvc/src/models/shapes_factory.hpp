#pragma once

#include <string>
#include <vector>

#include <models/shape.hpp>
#include <models/rectangle.hpp>


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

        if (shape_name == "rect") {
            Point lbc{params[0], params[1]};
            Point rtc{params[2], params[3]};
            auto ptr = std::make_shared<Rectangle>(lbc, rtc);
            return ShapePtr(std::move(ptr));
        }
        return nullptr;
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
        };
    }

    std::unordered_map<std::string, std::vector<std::string>> points_map_;
};

} // namespace mvc::shapes
