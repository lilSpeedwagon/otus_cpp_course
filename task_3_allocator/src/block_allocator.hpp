#pragma once

#include <memory>
#include <bitset>

#include <iostream> // TODO remove

namespace allocators {

template<typename T, size_t BlockSize>
class BlockAllocator {
public:
    using value_type = T;
    using BlockBitmap = std::bitset<BlockSize>;

    static constexpr const size_t block_size = BlockSize;

    // sequental block of memory with fixed size of T
    struct MemoryBlock {
        // marks non-empty cells
        BlockBitmap free_flags;
        // data in block 
        T buffer[block_size];
        // pointer to the next MemoryBlock
        MemoryBlock* next_ptr;

        // Returns pointer to the available segment of memory of size n,
        // nullptr if no such segment is available.
        T* acquire_free_segment(size_t n) {
            if (free_flags.all()) {
                return nullptr;
            }
            size_t free_sequence_size = 0;
            for (size_t i = 0; i < free_flags.size(); i++) {
                if (!free_flags[i]) {
                    if (++free_sequence_size == n) {
                        size_t from = i - n + 1;
                        set_flags(from, n, true);
                        return buffer + from;
                    }
                } else {
                    free_sequence_size = 0;
                }
            }
            return nullptr;
        }

        void release_segment(T* ptr, size_t n) {
            set_flags(ptr - buffer, n, false);
        }

        void set_flags(size_t from, size_t count, bool value) {
            for (size_t i = from; i < from + count; i++) {
                free_flags[i] = value;
            }
        }

        bool has_memory_segment(T* ptr, size_t n) {
            auto buffer_ptr = static_cast<T*>(buffer);
            return ptr >= buffer_ptr && ptr + n <= buffer_ptr + block_size;
        }

        bool empty() {
            return free_flags.none();
        }
    };

    BlockAllocator() noexcept 
        : blocks_allocated_{0}, size_{0}, blocks_list_head_{nullptr} {}
    
    ~BlockAllocator() {
        clear();
    }

    T* allocate (std::size_t n) {
        std::cout << "allocate(" << n << ")\n";
        if (n > block_size) {
            throw std::bad_alloc();
        }
        size_ += n;
        std::cout << "new size " << size_ << '\n';
        if (size_ > blocks_allocated_ * block_size) {
            std::cout << "it is not enough free memory, allocate new block\n";
            allocate_memory_block();
        }
        if (auto free_segment_ptr = find_free_segment(n);
            free_segment_ptr != nullptr) {
            std::cout << "free segment found\n";
            return free_segment_ptr;
        }
        std::cout << "free segment with enough size not found, allocate a new one\n";
        allocate_memory_block();
        return find_free_segment(n);
    }

    void deallocate(T* ptr, std::size_t n) {
        std::cout << "deallocate " << n << '\n';
        MemoryBlock* block_ptr = blocks_list_head_;
        MemoryBlock* prev_ptr = nullptr;
        while (block_ptr != nullptr) {
            if (block_ptr->has_memory_segment(ptr, n)) {
                block_ptr->release_segment(ptr, n);
                if (block_ptr->empty()) {
                    if (prev_ptr == nullptr && block_ptr == blocks_list_head_) {
                        blocks_list_head_ = nullptr;
                    } else {
                        prev_ptr->next_ptr = block_ptr->next_ptr;
                    }
                    delete block_ptr;
                    blocks_allocated_--;
                }
                if (n > size_) {
                    size_ = 0;
                } else {
                    size_ -= n;
                }
                return;
            }
            prev_ptr = block_ptr;
            block_ptr = block_ptr->next_ptr;
        }
        throw std::logic_error("memory is corrupted or allocated by other allocator");
    }

    void clear() {
        auto block_ptr = blocks_list_head_;
        while (block_ptr != nullptr) {
            auto to_delete = block_ptr;
            block_ptr = block_ptr->next_ptr;
            delete to_delete;
        }
        blocks_allocated_ = 0;
        size_ = 0;
        blocks_list_head_ = nullptr;
    }

    void print() {
        std::cout << "size " << size_ << '/' << blocks_allocated_ * BlockSize << '\n';
        auto block_ptr = blocks_list_head_;
        for (int i = 0; block_ptr != nullptr; i++) {
            std::cout << "Block " << i << ": " << block_ptr->free_flags;
            block_ptr = block_ptr->next_ptr;
        }
        std::cout << std::endl;
    }

    size_t size() const {
        return size_;
    }

    size_t blocks_allocated() const {
        return blocks_allocated_;
    }

private:
    static constexpr const size_t real_block_size = sizeof(MemoryBlock);

    MemoryBlock* allocate_memory_block() {
        auto block_ptr = new MemoryBlock{};
        block_ptr->next_ptr = blocks_list_head_;
        blocks_list_head_ = block_ptr;
        blocks_allocated_++;
        return block_ptr;
    }

    void deallocate_memory_block(MemoryBlock* block) {
        delete block;
        blocks_allocated_--;
    }

    T* find_free_segment(size_t n) {
        auto current_block_ptr = blocks_list_head_;
        while (current_block_ptr != nullptr) {
            if (auto ptr = current_block_ptr->acquire_free_segment(n); ptr != nullptr) {
                return ptr;
            }
            current_block_ptr = current_block_ptr->next_ptr;
        }
        return nullptr;
    }

    size_t blocks_allocated_;
    size_t size_;
    MemoryBlock* blocks_list_head_;
};

template<typename T>
class BlockAllocator<T, 0> {};

} // allocators
