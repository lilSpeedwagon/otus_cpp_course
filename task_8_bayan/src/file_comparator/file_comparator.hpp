#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>

#include <file_reader/factory.hpp>
#include <hash/hash.hpp>


namespace bayan::file {

using DuplicateGroups = std::unordered_map<hash::Hash, std::unordered_set<boost::filesystem::path>>;

/// @brief File comparator finding duplicates based on a file hash.
class FileComparator : public boost::noncopyable {
public:
    FileComparator(FileHashReaderFactory&& factory);
    ~FileComparator() = default;

    /// @brief Compare each file in list and find groups of duplicates.
    /// @param files list of files
    /// @return mapping hash -> list of duplicate files
    DuplicateGroups Compare(const std::vector<boost::filesystem::path>& files);

private:
    FileComparator(FileComparator&&);
    FileComparator& operator=(FileComparator&&);

    FileHashReaderFactory factory_;
};

} // namespace bayan::file
