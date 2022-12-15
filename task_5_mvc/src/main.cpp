#include <iostream>
#include <memory>

#include <editor/editor.hpp>
#include <models/document.hpp>
#include <models/point.hpp>
#include <models/shape.hpp>
#include <render/display_render.hpp>
#include <storage/file_storage.hpp>

namespace mvc {

std::unique_ptr<controller::Editor> InitEditor() {
    auto storage_ptr = std::make_shared<documents::FileStorage>();
    return std::make_unique<controller::Editor>(storage_ptr);
}

} // namespace mvc

int main() {
    auto editor_ptr = mvc::InitEditor();

    // TODO event loop with IO commands

    return 0;
}
