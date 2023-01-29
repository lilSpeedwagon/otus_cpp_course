#pragma once

#include <boost/filesystem/path.hpp>

#include <hash/hash_processor.hpp>


namespace bayan::file {

class FileHashReader {
public:
    FileHashReader() {}
    ~FileHashReader() {}

private:
    boost::filesystem::path path_;
    std::shared_ptr<hash::HashProcessor> hasher_;

    // hash
    // hash algo
    // block size
    // lazy loading + hash computing for loaded data blocks
};

class FileHashReaderFactory {
public:
    FileHashReaderFactory() {}
    ~FileHashReaderFactory() {}

    FileHashReader Create(const boost::filesystem::path& path) const {
        return FileHashReader();
    }

private:
    // hash algo
    // block size
};

} // bayan::file
