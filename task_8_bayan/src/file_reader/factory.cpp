#include "factory.hpp"


namespace bayan::file {

FileHashReaderFactory::FileHashReaderFactory(
    hash::HashProcessorFactory&& hash_factory, size_t block_size)
    : hash_factory_(std::move(hash_factory)), block_size_(block_size) {}

FileHashReaderFactory::FileHashReaderFactory(FileHashReaderFactory&& other) 
    : hash_factory_(std::move(other.hash_factory_)),
      block_size_(std::move(other.block_size_)) {}

FileHashReaderFactory& FileHashReaderFactory::operator=(
    FileHashReaderFactory&& other) {
    std::swap(hash_factory_, other.hash_factory_);
    std::swap(block_size_, other.block_size_);
    return *this;
}

FileHashReader FileHashReaderFactory::Create(
    const boost::filesystem::path& path) {
    auto hasher_ptr = hash_factory_.Create();
    return FileHashReader(path, hasher_ptr, block_size_);
}

} // bayan::file