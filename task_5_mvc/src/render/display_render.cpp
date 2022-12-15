#include "display_render.hpp"

#include <iostream>

#include <models/point.hpp>


namespace mvc::render {

std::shared_ptr<DisplayRenderEngine> DisplayRenderEngine::GetInstance() {
    if (instance_ptr == nullptr) {
        instance_ptr = std::make_shared<DisplayRenderEngine>();
    }
    return instance_ptr;
}
    
DisplayRenderEngine::~DisplayRenderEngine() {}

void DisplayRenderEngine::RenderShapes(const std::vector<shapes::ShapePtr>& shapes) {
    std::cout << "-------- Render start ---------";
    for (const auto& shape : shapes) {
        RenderShape(shape);
    }
    std::cout << "--------- Render end ----------";
}

DisplayRenderEngine::DisplayRenderEngine() {}

void DisplayRenderEngine::RenderShape(const shapes::ShapePtr& shape_ptr) {
    std::cout << "Rendering " << shape_ptr->GetInfo() << ": ";
    const auto& points = shape_ptr->GetPoints();
    for (size_t i = 0; i < points.size(); i++) {
        std::cout << '(' << points[i].x << ';' << points[i].y << ')';
        if (i < points.size() - 1) {
            std::cout << "->";
        }
        std::cout << '\n';
    }
}

} // namespace mvc::render
