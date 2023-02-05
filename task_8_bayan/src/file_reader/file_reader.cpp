#include "file_reader.hpp"


#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>

#include <hash/hash.hpp>
#include <hash/hash_processor.hpp>
#include <hash/factory.hpp>
#include <utils.hpp>


namespace bayan::file {

FileHashReader::FileHashReader(
    const boost::filesystem::path& path,
    std::shared_ptr<hash::HashProcessor> hasher_ptr,
    size_t block_size)
    : path_(path), hasher_ptr_(hasher_ptr), block_size_(block_size),
      file_size_(boost::filesystem::file_size(path_)), blocks_loaded_(0),
      current_hash_(), is_full_hash_(false) {}

FileHashReader::FileHashReader(FileHashReader&& other) {
    Swap(std::move(other));
}

FileHashReader& FileHashReader::operator=(FileHashReader&& other) {
    Swap(std::move(other));
    return *this;
}

void FileHashReader::Swap(FileHashReader&& other) {
    std::swap(path_, other.path_);
    std::swap(hasher_ptr_, other.hasher_ptr_);
    std::swap(block_size_, other.block_size_);
    std::swap(file_size_, other.file_size_);
    std::swap(blocks_loaded_, other.blocks_loaded_);
    std::swap(current_hash_, other.current_hash_);
    std::swap(is_full_hash_, other.is_full_hash_);
}

bool FileHashReader::operator==(FileHashReader& other) {
    return Compare(other);
}

hash::Hash FileHashReader::GetHash() const {
    return current_hash_;
}

boost::filesystem::path FileHashReader::GetPath() const {
    return path_;
}

bool FileHashReader::Compare(FileHashReader& other) {
    if (file_size_ != other.file_size_) {
        return false;
    }
    if (is_full_hash_ && other.is_full_hash_) {
        return current_hash_ == other.current_hash_;
    }

    boost::filesystem::ifstream lhs_stream(path_);
    boost::filesystem::ifstream rhs_stream(other.path_);

    // align loaded blocks in both files
    while (blocks_loaded_ != other.blocks_loaded_) {
        if (blocks_loaded_ < other.blocks_loaded_) {
            ReadNextBlock(lhs_stream);
        } else {
            other.ReadNextBlock(rhs_stream);
        }
    }

    // calculate hashes block-by-block until some differences or EOF
    while (!is_full_hash_ && current_hash_ == other.current_hash_) {
        ReadNextBlock(lhs_stream);
        other.ReadNextBlock(rhs_stream);
    }
    return current_hash_ == other.current_hash_;
}

void FileHashReader::ReadNextBlock(boost::filesystem::ifstream& stream) {
    size_t cursor = block_size_ * blocks_loaded_;
    size_t buffer_size = std::min(block_size_, file_size_ - cursor);

    std::string buffer;
    buffer.resize(buffer_size);
    stream.seekg(cursor);
    stream.read(buffer.data(), block_size_);

    auto hash = hasher_ptr_->GetHash(buffer);
    current_hash_ = hash::HashCombine(current_hash_, hash);
    blocks_loaded_++;

    if (cursor + buffer_size >= file_size_) {
        is_full_hash_ = true;
    }
}

} // bayan::file
