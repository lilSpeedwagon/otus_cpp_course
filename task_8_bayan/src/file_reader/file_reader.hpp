#pragma once

#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>

#include <hash/hash.hpp>
#include <hash/hash_processor.hpp>
#include <hash/factory.hpp>
#include <utils.hpp>


namespace bayan::file {

class FileHashReader {
public:
    FileHashReader(
        const boost::filesystem::path& path,
        std::shared_ptr<hash::HashProcessor> hasher_ptr,
        size_t block_size) 
        : path_(path), hasher_ptr_(hasher_ptr), block_size_(block_size),
          file_size_(boost::filesystem::file_size(path_)), blocks_loaded_(0),
          current_hash_(), is_full_hash_(false) {}
    ~FileHashReader() {}

    bool operator==(FileHashReader& other) {
        return Compare(other);
    }

private:
    bool Compare(FileHashReader& other) {
        if (file_size_ != other.file_size_) {
            return false;
        }
        if (is_full_hash_ && other.is_full_hash_) {
            return current_hash_ == other.current_hash_;
        }

        boost::filesystem::ifstream lhs_stream(path_);
        boost::filesystem::ifstream rhs_stream(other.path_);

        // align loaded blocks
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

    void ReadNextBlock(boost::filesystem::ifstream& stream) {
        size_t cursor = block_size_ * blocks_loaded_;
        size_t buffer_size = std::min(block_size_, file_size_ - cursor);

        std::string buffer;
        buffer.resize(buffer_size);
        stream.seekg(cursor);
        stream.read(buffer.data(), block_size_);

        auto hash = hasher_ptr_->GetHash(buffer);
        hash::HashCombine(current_hash_, hash);
        blocks_loaded_++;

        if (cursor + buffer_size >= file_size_) {
            is_full_hash_ = true;
        }
    }

    const boost::filesystem::path path_;
    const std::shared_ptr<hash::HashProcessor> hasher_ptr_;
    const size_t block_size_;

    size_t file_size_;
    size_t blocks_loaded_;
    hash::Hash current_hash_;
    bool is_full_hash_;
};

} // bayan::file
