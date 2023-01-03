#include "document.hpp"

#include <algorithm>

#include <render/display_render.hpp>


namespace mvc::documents {

Document::Document() : id_counter_(0) {}

Document::~Document() {}

shapes::ShapeId Document::AddShape(shapes::ShapePtr shape_ptr) {
    auto id = GetNextId();
    shapes_[id] = shape_ptr;
    Render();
    return id;
}

bool Document::DeleteShape(const shapes::ShapeId& id) {
    if (shapes_.erase(id) != 0) {
        Render();
        return true;
    }
    return false;
}

void Document::Render() {
    std::vector<shapes::ShapePtr> shapes_list;
    shapes_list.reserve(shapes_.size());
    std::transform(shapes_.begin(), shapes_.end(),
                    std::back_inserter(shapes_list),
                    [](const auto& kv) { return kv.second; });
    render::DisplayRenderEngine::GetInstance()->RenderShapes(shapes_list);
}

shapes::ShapeId Document::GetNextId() {
    return shapes::ShapeId{id_counter_++};
}

} // namespace mvc::documents
