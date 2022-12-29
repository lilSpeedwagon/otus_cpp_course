#pragma once

#include <vector>

#include <boost/noncopyable.hpp>

#include <models/shape.hpp>


namespace mvc::render {

/// @brief Engine for rendering shapes to the display. Singleton class.
class DisplayRenderEngine : public boost::noncopyable {
public:
    /// @brief Get an instance of RenderEngine. Based on lazy loading approach.
    static DisplayRenderEngine* GetInstance();
    
    ~DisplayRenderEngine();

    /// @brief Render array of shapes to the display
    void RenderShapes(const std::vector<shapes::ShapePtr>& shapes);

private:
    DisplayRenderEngine();
    void RenderShape(const shapes::ShapePtr& shape_ptr);

    static DisplayRenderEngine* instance_ptr;
};

} // namespace mvc::render
