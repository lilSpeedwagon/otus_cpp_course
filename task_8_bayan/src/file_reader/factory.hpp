#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>

#include <file_reader/file_reader.hpp>
#include <hash/factory.hpp>


namespace bayan::file {

/// @brief FileHashReader factory
class FileHashReaderFactory : public boost::noncopyable {
public:
    /// @brief Constructor
    /// @param hash_factory HashProcessor factory 
    /// @param block_size size of buffer to read files
    FileHashReaderFactory(hash::HashProcessorFactory&& hash_factory, size_t block_size);
    FileHashReaderFactory(FileHashReaderFactory&& other);
    ~FileHashReaderFactory() = default;

    FileHashReaderFactory& operator=(FileHashReaderFactory&& other);

    /// @brief Create FileHashReader instance
    /// @param path path to the readed file
    /// @return created FileHashReader instance
    FileHashReader Create(const boost::filesystem::path& path);

private:
    hash::HashProcessorFactory hash_factory_;
    size_t block_size_;
};

} // bayan::file