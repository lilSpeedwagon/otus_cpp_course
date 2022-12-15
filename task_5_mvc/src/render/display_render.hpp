#pragma once

#include <memory>
#include <vector>

#include <boost/noncopyable.hpp>

#include <models/shape.hpp>
#include <render/render.hpp>


namespace mvc::render {

/// @brief Engine for rendering shapes to the display. Singleton class.
class DisplayRenderEngine : public boost::noncopyable, RenderEngine {
public:
    /// @brief Get an instance of RenderEngine. Based on lazy loading approach.
    static std::shared_ptr<DisplayRenderEngine> GetInstance();
    
    virtual ~DisplayRenderEngine();

    /// @brief Render array of shapes to the display
    void RenderShapes(const std::vector<shapes::ShapePtr>& shapes) override;

private:
    DisplayRenderEngine();
    void RenderShape(const shapes::ShapePtr& shape_ptr);

    static std::shared_ptr<DisplayRenderEngine> instance_ptr;
};

} // namespace mvc::render
