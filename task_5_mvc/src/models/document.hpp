#pragma once

#include <memory>
#include <unordered_map>

#include <models/shape.hpp>
#include <utils/typedef.hpp>


namespace mvc::documents {

/// @brief Document containing vector shapes.
class Document {
public:
    Document();
    ~Document();

    /// @brief Adds a shape to the document.
    /// @param shape_ptr pointer to the shape
    /// @return id of the added shape
    shapes::ShapeId AddShape(shapes::ShapePtr shape_ptr);

    /// @brief Deletes shape from the document.
    /// @param id id of the deletable shape
    /// @return true if shape is deleted
    bool DeleteShape(const shapes::ShapeId& id);

    /// @brief Triggers force rendering of the document.
    void Render();

private:
    shapes::ShapeId GetNextId();

    size_t id_counter_;
    std::unordered_map<shapes::ShapeId, shapes::ShapePtr> shapes_;
};

using DocumentPtr = std::shared_ptr<Document>;
using DocumentId = utils::StrongTypedef<size_t, struct DocumentIdTag>;

} // namespace mvc::documents
