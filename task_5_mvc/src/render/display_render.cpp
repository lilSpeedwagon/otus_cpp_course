#include "display_render.hpp"

#include <iostream>

#include <models/point.hpp>


namespace mvc::render {

DisplayRenderEngine* DisplayRenderEngine::instance_ptr = nullptr;

DisplayRenderEngine* DisplayRenderEngine::GetInstance() {
    if (instance_ptr == nullptr) {
        instance_ptr = new DisplayRenderEngine();
    }
    return instance_ptr;
}
    
DisplayRenderEngine::DisplayRenderEngine() {}

DisplayRenderEngine::~DisplayRenderEngine() {}

void DisplayRenderEngine::RenderShapes(const std::vector<shapes::ShapePtr>& shapes) {
    std::cout << "-------- Render start ---------\n";
    for (const auto& shape : shapes) {
        RenderShape(shape);
    }
    std::cout << "--------- Render end ----------\n";
}

void DisplayRenderEngine::RenderShape(const shapes::ShapePtr& shape_ptr) {
    std::cout << "Rendering " << shape_ptr->GetInfo() << ": ";
    const auto& points = shape_ptr->GetPoints();
    for (size_t i = 0; i < points.size(); i++) {
        std::cout << '(' << points[i].x << ';' << points[i].y << ')';
        if (i < points.size() - 1) {
            std::cout << "->";
        }
    }
}

} // namespace mvc::render
