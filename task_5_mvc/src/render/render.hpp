#pragma once

#include <vector>

#include <models/shape.hpp>


namespace mvc::render {

/// @brief Engine interface for shapes rendering.
class RenderEngine {
public:
    RenderEngine() {}
    virtual ~RenderEngine() {}

    /// @brief Render shapes
    /// @param shapes vector of shapes to render
    virtual void RenderShapes(const std::vector<shapes::ShapePtr>& shapes) = 0;
};

} // namespace mvc::render
