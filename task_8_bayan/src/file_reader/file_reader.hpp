#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>

#include <hash/hash_processor.hpp>
#include <hash/factory.hpp>


namespace bayan::file {

class FileHashReader {
public:
    FileHashReader(
        const boost::filesystem::path& path,
        std::shared_ptr<hash::HashProcessor> hasher_ptr,
        size_t block_size) 
        : path_(path), hasher_ptr_(hasher_ptr), block_size_(block_size) {}
    ~FileHashReader() {}

private:
    boost::filesystem::path path_;
    std::shared_ptr<hash::HashProcessor> hasher_ptr_;
    size_t block_size_;
    // lazy loading + hash computing for loaded data blocks
};

class FileHashReaderFactory : public boost::noncopyable {
public:
    FileHashReaderFactory(hash::HashProcessorFactory&& hash_factory, size_t block_size)
        : hash_factory_(std::move(hash_factory)), block_size_(block_size) {}
    FileHashReaderFactory(FileHashReaderFactory&& other) 
        : hash_factory_(std::move(other.hash_factory_)),
          block_size_(std::move(other.block_size_)) {}
    ~FileHashReaderFactory() {}

    FileHashReaderFactory& operator=(FileHashReaderFactory&& other) {
        std::swap(hash_factory_, other.hash_factory_);
        std::swap(block_size_, other.block_size_);
        return *this;
    }

    FileHashReader Create(const boost::filesystem::path& path) {
        auto hasher_ptr = hash_factory_.Create();
        return FileHashReader(path, hasher_ptr, block_size_);
    }

private:

    hash::HashProcessorFactory hash_factory_;
    size_t block_size_;
};

} // bayan::file
