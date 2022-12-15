#pragma once

#include <string>
#include <unordered_map>

#include <boost/noncopyable.hpp>

#include <models/document.hpp>
#include <storage/storage.hpp>


namespace mvc::documents {

/// @brief Documents storage based on the filesystem.
class FileStorage : public Storage, public boost::noncopyable {
public: 
    FileStorage() {}
    virtual ~FileStorage() {}

    /// @brief Stores a new document to the storage.
    /// @param name name of the document
    /// @param document pointer to the document
    void AddDocument(const std::string& name, DocumentPtr document) override {
        documents_[name] = document;
    }

    /// @brief Gets document by its name.
    /// @param name name of the document
    /// @return pointer to the document if found, else an empty pointer
    DocumentPtr GetDocument(const std::string& name) const override {
        if (auto it = documents_.find(name);
            it != documents_.end()) {
            return it->second;
        }
        return nullptr;
    }

private:
    std::unordered_map<std::string, DocumentPtr> documents_;
};

} // namespace mvc::documents
