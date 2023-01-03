#pragma once

#include <string>
#include <unordered_map>

#include <boost/noncopyable.hpp>

#include <models/document.hpp>
#include <storage/storage.hpp>


namespace mvc::documents {

/// @brief Documents storage based on the filesystem.
/// File system is mocked with std::unordered_map for testing purposes.
class FileStorage : public Storage, public boost::noncopyable {
public: 
    FileStorage();
    FileStorage(FileStorage&& other);
    virtual ~FileStorage();

    FileStorage& operator=(FileStorage&& other);

    /// @brief Stores a new document to the storage.
    /// @param name name of the document
    /// @param document pointer to the document
    void AddDocument(const std::string& name, DocumentPtr document) override;

    /// @brief Gets document by its name.
    /// @param name name of the document
    /// @return pointer to the document if found, else an empty pointer
    DocumentPtr GetDocument(const std::string& name) const override;

private:
    std::unordered_map<std::string, DocumentPtr> documents_;
};

} // namespace mvc::documents
