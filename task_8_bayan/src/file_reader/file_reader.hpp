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

} // bayan::file
