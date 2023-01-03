#pragma once

#include <memory>
#include <optional>

#include <boost/noncopyable.hpp>

#include <models/document.hpp>
#include <storage/storage.hpp>


namespace mvc::controller {

/// @brief Controller of the vector graphics editor.
class Editor : public boost::noncopyable {
public:
    /// @brief Constructor.
    /// @param storage_ptr pointer to the documents storage 
    Editor(std::shared_ptr<documents::Storage> storage_ptr);
    Editor(Editor&& other);
    ~Editor();

    Editor& operator=(Editor&& other);

    /// @brief Closes current document and creates a new one.
    void CreateDocument();
    
    /// @brief Imports document from storage by name.
    /// @param name name of the document
    /// @return whether a document found and imported
    bool ImportDocument(const std::string& name);

    /// @brief Stores the current document to the storage by name.
    /// @param name name of a document to store it with
    /// @return whether the current document is present and exported 
    bool ExportDocument(const std::string& name);
    
    /// @brief Closes the current document.
    /// @returns Whether the current document is closed. 
    bool CloseDocument();

    /// @brief Gets current document.
    /// @return pointer to the current document
    documents::DocumentPtr GetCurrentDocument() const;

    /// @brief Gets current document name.
    /// @return name of the current document if some is opened, else std::nullopt
    std::optional<std::string> GetCurrentDocumentName() const;

private:
    void Swap(Editor&& other);

    std::optional<std::string> current_doc_name_;
    documents::DocumentPtr current_doc_ptr_;
    std::shared_ptr<documents::Storage> storage_ptr_;
};

} // namespace mvc::controller
