#pragma once

#include <string>

#include <models/document.hpp>


namespace mvc::documents {

/// @brief Abstract storage for the documents.
class Storage {
public: 
    Storage() {}
    virtual ~Storage() {}
    
    /// @brief Stores a new document to the storage.
    /// @param name name of the document
    /// @param document pointer to the document
    virtual void AddDocument(const std::string& name, DocumentPtr document) = 0;
    
    /// @brief Gets document by its name.
    /// @param name name of the document
    /// @return pointer to the document if found, else an empty pointer
    virtual DocumentPtr GetDocument(const std::string& name) const = 0;
};

} // namespace mvc::documents
