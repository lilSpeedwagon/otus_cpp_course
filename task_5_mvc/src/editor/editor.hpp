#pragma once

#include <memory>
#include <optional>

#include <boost/noncopyable.hpp>

#include <models/document.hpp>
#include <storage/storage.hpp>
#include <render/render.hpp>


namespace mvc::controller {

/// @brief Controller of the vector graphics editor.
class Editor : public boost::noncopyable {
public:
    /// @brief Constructor.
    /// @param storage_ptr pointer to the documents storage 
    Editor(std::shared_ptr<documents::Storage> storage_ptr)
        : storage_ptr_(storage_ptr), current_doc_name_(), current_doc_ptr_() {}
    ~Editor() {}

    /// @brief Closes current document and creates a new one.
    void CreateDocument() {
        current_doc_name_ = "Untitled";
        current_doc_ptr_ = std::make_shared<documents::Document>();
    }
    
    /// @brief Imports document from storage by name.
    /// @param name name of the document
    /// @return whether a document found and imported
    bool ImportDocument(const std::string& name) {
        current_doc_ptr_ = storage_ptr_->GetDocument(name);
        if (current_doc_ptr_ != nullptr) {
            current_doc_name_ = name;
            return true;
        }
        return false;
    }

    /// @brief Stores the current document to the storage by name.
    /// @param name name of a document to store it with
    /// @return whether the current document is present and exported 
    bool ExportDocument(const std::string& name) {
        if (current_doc_ptr_ != nullptr) {
            storage_ptr_->AddDocument(name, current_doc_ptr_);
            current_doc_name_ = name;
            return true;
        }
        return false;
    }
    
    /// @brief Closes the current document.
    void CloseDocument() {
        current_doc_ptr_.reset();
        current_doc_name_.reset();
    }

    /// @brief Gets current document.
    /// @return pointer to the current document
    documents::DocumentPtr GetCurrentDocument() const {
        return current_doc_ptr_;
    }

    /// @brief Gets current document name.
    /// @return name of the current document if some is opened, else std::nullopt
    std::optional<std::string> GetCurrentDocumentName() const {
        return current_doc_name_;
    }

private:
    std::optional<std::string> current_doc_name_;
    documents::DocumentPtr current_doc_ptr_;
    std::shared_ptr<documents::Storage> storage_ptr_;
};

} // namespace mvc::controller
