#include "editor.hpp"


namespace mvc::controller {

Editor::Editor(std::shared_ptr<documents::Storage> storage_ptr)
    : current_doc_name_(), current_doc_ptr_(), storage_ptr_(storage_ptr) {}

Editor::~Editor() {}

Editor::Editor(Editor&& other) {
    Swap(std::move(other));
}

Editor& Editor::operator=(Editor&& other) {
    Swap(std::move(other));
    return *this;
}

void Editor::CreateDocument() {
    current_doc_name_ = "Untitled";
    current_doc_ptr_ = std::make_shared<documents::Document>();
}

bool Editor::ImportDocument(const std::string& name) {
    auto doc_ptr = storage_ptr_->GetDocument(name);
    if (doc_ptr != nullptr) {
        current_doc_ptr_ = doc_ptr;
        current_doc_name_ = name;
        return true;
    }
    return false;
}

bool Editor::ExportDocument(const std::string& name) {
    if (current_doc_ptr_ != nullptr) {
        storage_ptr_->AddDocument(name, current_doc_ptr_);
        current_doc_name_ = name;
        return true;
    }
    return false;
}

bool Editor::CloseDocument() {
    bool result = current_doc_ptr_ != nullptr;
    current_doc_ptr_.reset();
    current_doc_name_.reset();
    return result;
}

documents::DocumentPtr Editor::GetCurrentDocument() const {
    return current_doc_ptr_;
}

std::optional<std::string> Editor::GetCurrentDocumentName() const {
    return current_doc_name_;
}

void Editor::Swap(Editor&& other) {
    std::swap(current_doc_name_, other.current_doc_name_);
    std::swap(current_doc_ptr_, other.current_doc_ptr_);
    storage_ptr_ = other.storage_ptr_;
}

} // namespace mvc::controller
