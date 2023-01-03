#include "file_storage.hpp"


namespace mvc::documents {

FileStorage::FileStorage() {}

FileStorage::~FileStorage() {}

FileStorage::FileStorage(FileStorage&& other) {
    std::swap(documents_, other.documents_);
}

FileStorage& FileStorage::operator=(FileStorage&& other) {
    std::swap(documents_, other.documents_);
    return *this;
}

void FileStorage::AddDocument(const std::string& name, DocumentPtr document) {
    documents_[name] = document;
}

DocumentPtr FileStorage::GetDocument(const std::string& name) const {
    if (auto it = documents_.find(name);
        it != documents_.end()) {
        return it->second;
    }
    return nullptr;
}

} // namespace mvc::documents
