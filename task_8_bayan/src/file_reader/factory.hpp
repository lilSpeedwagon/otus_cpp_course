#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>

#include <file_reader/file_reader.hpp>
#include <hash/factory.hpp>


namespace bayan::file {

class FileHashReaderFactory : public boost::noncopyable {
public:
    FileHashReaderFactory(hash::HashProcessorFactory&& hash_factory, size_t block_size);
    FileHashReaderFactory(FileHashReaderFactory&& other);
    ~FileHashReaderFactory();

    FileHashReaderFactory& operator=(FileHashReaderFactory&& other);

    FileHashReader Create(const boost::filesystem::path& path);

private:
    hash::HashProcessorFactory hash_factory_;
    size_t block_size_;
};

} // bayan::file