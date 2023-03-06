#pragma once

#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>

#include <hash/hash.hpp>
#include <hash/hash_processor.hpp>
#include <hash/factory.hpp>
#include <utils.hpp>


namespace bayan::file {

/// @brief File wrapper to compare the content of files by its hash.
/// Hash calculation is lazy and is performed only if it is required for
/// comparison.
class FileHashReader : public boost::noncopyable {
public:
    /// @brief Constructor
    /// @param path file path
    /// @param hasher_ptr pointer to the HashProcessor
    /// @param block_size size of file content buffer
    FileHashReader(const boost::filesystem::path& path,
                   std::shared_ptr<hash::HashProcessor> hasher_ptr,
                   size_t block_size);
    FileHashReader(FileHashReader&& other);
    ~FileHashReader() = default;

    FileHashReader& operator=(FileHashReader&& other);

    /// @brief Compares two files by their hashes. If some differences found or
    /// if size of files are different - further hash calculation is omitted. 
    /// @param other file to compare with
    /// @return whether the file contents are equal
    bool operator==(FileHashReader& other);

    /// @brief Returns hash of loaded file content
    hash::Hash GetHash() const;

    /// @brief Returns file path
    boost::filesystem::path GetPath() const;

private:
    void Swap(FileHashReader&& other);
    bool Compare(FileHashReader& other);
    void ReadNextBlock(boost::filesystem::ifstream& stream);

    boost::filesystem::path path_;
    std::shared_ptr<hash::HashProcessor> hasher_ptr_;
    size_t block_size_;

    size_t file_size_;
    size_t blocks_loaded_;
    hash::Hash current_hash_;
    bool is_full_hash_;
};

} // bayan::file
