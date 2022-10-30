#pragma once

#include <memory>
#include <bitset>

namespace allocators {

template<typename T, size_t BlockSize>
class BlockAllocator : std::allocator_traits<BlockAllocator> {
public:
    using value_type = T;

    static constexpr const size_t block_size = BlockSize;

    // sequental block of memory with fixed size of T
    struct MemoryBlock {
        // marks non-empty cells
        BlockBitmap free_flags_;
        // data in block 
        T buffer_[block_size];
        // pointer to the next MemoryBlock
        MemoryBlock* next_ptr;

        // Returns pointer to the available segment of memory of size n,
        // nullptr if no such segment is available.
        T* find_free_segment(size_t n) {
            if (free_flags_.none()) {
                return nullptr;
            }
            size_t free_sequence_size = 0;
            for (size_t i = 0; i < free_flags_.size(); i++) {
                if (!free_flags_[i]) {
                    if (++free_sequence_size == n) {
                        return buffer_ + i - (n - 1);
                    }
                } else {
                    free_sequence_size = 0;
                }
            }
            return nullptr;
        }
    };

    BlockAllocator() noexcept : blocks_allocated_{0}, size_{0}, blocks_list_head_{nullptr} {}
    ~BlockAllocator() {}

    T* allocate (std::size_t n) {
        if (n > block_size) {
            throw std::bad_alloc();
        }

        if (size_ + n >= blocks_allocated_ * block_size) {
            return allocate_memory_block()->buffer;
        }

        auto block_ptr = blocks_list_head_;
        while (block_ptr != nullptr) {
            if (auto segment_ptr = block_ptr->get_free_segment_of_size(n);
                segment_ptr != nullptr) {
                return segment_ptr;
            }
        }

        return allocate_memory_block()->buffer;
    }

    void deallocate (T* p, std::size_t n) {
        // how to find MemoryBlock?
        // sequental lookup and address math ???
        // free memory, mark memory block segment as empty
        // if memory block is empty - deallocate block
    }

private:
    using BlockBitmap = std::bitset<BlockSize>;
    static constexpr const size_t real_block_size = sizeof(MemoryBlock);

    MemoryBlock* allocate_memory_block() {
        auto block_ptr = new MemoryBlock{};
        block_ptr->next_ptr = blocks_list_head_;
        blocks_list_head_ = block_ptr;
        return block_ptr;
    }

    MemoryBlock* find_free_block(size_t n) {
        auto current_block_ptr = blocks_list_head_;
        while (current_block_ptr != nullptr) {
            if (current_block_ptr->find_free_segment)
        }
        return nullptr;
    }

    size_t blocks_allocated_;
    size_t size_;
    MemoryBlock* blocks_list_head_;
};

} // allocators
