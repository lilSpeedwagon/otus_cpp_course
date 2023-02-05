#include "file_comparator.hpp"


namespace bayan::file {

FileComparator::FileComparator(FileHashReaderFactory&& factory) : factory_(std::move(factory)) {}

DuplicateGroups FileComparator::Compare(const std::vector<boost::filesystem::path>& files) {
    std::vector<FileHashReader> hash_readers;
    hash_readers.reserve(files.size());
    for (const auto& file_path : files) {
        auto hash_reader = factory_.Create(file_path);
        hash_readers.push_back(std::move(hash_reader));
    }
    
    DuplicateGroups result;
    for (size_t i = 0; i < hash_readers.size() - 1; i++) {
        auto& lhs = hash_readers[i];
        for (size_t j = i + 1; j < hash_readers.size(); j++) {
            auto& rhs = hash_readers[j];
            if (lhs == rhs) {
                auto hash = lhs.GetHash();
                result[hash].insert(lhs.GetPath());
                result[hash].insert(rhs.GetPath());
            }
        }
    }
    return result;
}

} // namespace bayan::file
