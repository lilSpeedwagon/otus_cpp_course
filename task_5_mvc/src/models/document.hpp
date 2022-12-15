#pragma once

#include <algorithm>
#include <memory>
#include <unordered_map>

#include <models/shape.hpp>
#include <render/render.hpp>
#include <utils/typedef.hpp>


namespace mvc::documents {

/// @brief Document containing vector shapes.
class Document {
public:
    /// @brief Constructor.
    /// @param render_ptr Pointer to the target renderer
    Document() : id_counter_(0) {}
    ~Document() {}

    /// @brief Adds a shape to the document.
    /// @param shape_ptr pointer to the shape
    /// @return id of the added shape
    shapes::ShapeId AddShape(shapes::ShapePtr shape_ptr) {
        auto id = GetNextId();
        shapes_[id] = shape_ptr;
        OnUpdate();
        return id;
    }

    /// @brief Deletes shape from the document.
    /// @param id id of the deletable shape
    /// @return true if shap is deleted
    bool DeleteShape(const shapes::ShapeId& id) {
        const auto deleted_count = shapes_.erase(id);
        OnUpdate();
        return deleted_count != 0;
    }

private:
    void OnUpdate() {
        std::vector<shapes::ShapePtr> shapes_list;
        shapes_list.reserve(shapes_.size());
        std::transform(shapes_.begin(), shapes_.end(),
                       std::back_inserter(shapes_list),
                       [](const auto& kv) { return kv.second; });
        render::DisplayRenderEngine::GetInstance()->RenderShapes(shapes_list);
    }
    shapes::ShapeId GetNextId() {
        return shapes::ShapeId{id_counter_++};
    }

    size_t id_counter_;
    std::unordered_map<shapes::ShapeId, shapes::ShapePtr> shapes_;
};

using DocumentPtr = std::shared_ptr<Document>;
using DocumentId = utils::StrongTypedef<size_t, struct DocumentIdTag>;

} // namespace mvc::documents
