#pragma once

#include <memory>

#include <boost/noncopyable.hpp>

#include <models/document.hpp>
#include <storage/storage.hpp>
#include <render/render.hpp>


namespace mvc::controller {

/// @brief Controller of the vector graphics editor.
class Editor : public boost::noncopyable {
public:
    Editor(std::shared_ptr<documents::Storage> storage_ptr)
        : storage_ptr_(storage_ptr) {}
    ~Editor() {}

    /// @brief Closes current document and creates a new one.
    void CreateDocument() {
        current_doc_ptr_ = std::make_shared<documents::Document>();
    }
    
    /// @brief Imports document from storage by name.
    /// @param name name of the document
    /// @return whether a document found and imported
    bool ImportDocument(const std::string& name) {
        current_doc_ptr_ = storage_ptr_->GetDocument(name);
        return current_doc_ptr_ != nullptr;
    }

    /// @brief Stores the current document to the storage by name.
    /// @param name name of a document to store it with
    /// @return whether the current document is present and exported 
    bool ExportDocument(const std::string& name) {
        if (storage_ptr_ != nullptr) {
            storage_ptr_->AddDocument(name, current_doc_ptr_);
            return true;
        }
        return false;
    }
    
    /// @brief Closes the current document.
    void CloseDocument() {
        current_doc_ptr_.reset();
    }

    /// @brief Gets current document.
    /// @return pointer to the current document
    documents::DocumentPtr GetCurrentDocument() const {
        return current_doc_ptr_;
    }

private:
    documents::DocumentPtr current_doc_ptr_;
    std::shared_ptr<documents::Storage> storage_ptr_;
};

} // namespace mvc::controller
