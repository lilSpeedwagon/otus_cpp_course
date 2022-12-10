#pragma once

#include <memory>
#include <bitset>


namespace allocators {

/// @brief Allocator which allocates memory with blocks of constant size.
/// It helps to reduce OS memory manager calls even with many allocation
/// of short memory segments. Allocation of memory with size more than 
/// block size is prohibited.
/// @tparam T type of the allocated elements
/// @tparam BlockSize size of allocated block in terms of type T
template<typename T, size_t BlockSize>
class BlockAllocator {
private:
    // forward declaration
    using BlockBitmap = std::bitset<BlockSize>;
    static constexpr const size_t block_size = BlockSize;
    struct MemoryBlock;

public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using difference_type = ptrdiff_t;

    template <typename U>
    struct rebind {
		typedef BlockAllocator<U, BlockSize> other;
    };

    BlockAllocator() noexcept 
        : blocks_allocated_{0}, size_{0}, blocks_list_head_{nullptr} {}
    BlockAllocator(const BlockAllocator& other)
        : blocks_allocated_{0}, size_{0}, blocks_list_head_{nullptr} {
        clear();
        copy(other);
    }
    BlockAllocator& operator=(const BlockAllocator& other) {
        clear();
        copy(other);
        return *this;
    }
    BlockAllocator(BlockAllocator&& other)
        : blocks_allocated_{0}, size_{0}, blocks_list_head_{0} {
        swap(std::move(other));
    }
    BlockAllocator& operator=(BlockAllocator&& other) {
        swap(std::move(other));
        return *this;
    }
    ~BlockAllocator() {
        clear();
    }

    /// @brief Allocates sequental segment of memory.
    /// @param n Size of allocated segment in terms of type T
    /// @return Pointer to the beginning of the allocated segment. nullptr if n = 0.
    /// @throw std::bad_alloc if cannot allocate memory or size of
    /// the requested segment is bigger than size of memory block.
    pointer allocate (size_t n) {
        if (n == 0) {
            return nullptr;
        }
        if (n > block_size) {
            throw std::bad_alloc();
        }

        size_ += n;
        if (size_ > blocks_allocated_ * block_size) {
            allocate_memory_block();
        }
        if (auto free_segment_ptr = find_free_segment(n);
            free_segment_ptr != nullptr) {
            return free_segment_ptr;
        }
        allocate_memory_block();
        return find_free_segment(n);
    }

    /// @brief Deletes a segment of memory with size n.
    /// The segment may be not physically deleted but could be
    /// put back into the allocator memory pool and reused later.
    /// @param ptr pointer to the deletable memory.
    /// Must be obtained via the method allocate() of the corresponding allocator
    /// @param n size of the deletable memory segment.
    /// Must be the same as the size of the memory segment allocated via the allocate() method
    void deallocate(pointer ptr, std::size_t n) {
        MemoryBlock* block_ptr = blocks_list_head_;
        MemoryBlock* prev_ptr = nullptr;
        while (block_ptr != nullptr) {
            if (block_ptr->has_memory_segment(ptr, n)) {
                block_ptr->release_segment(ptr, n);
                // release memory block if it is empty
                if (block_ptr->empty()) {
                    if (prev_ptr == nullptr && block_ptr == blocks_list_head_) {
                        blocks_list_head_ = nullptr;
                    } else {
                        prev_ptr->next_ptr = block_ptr->next_ptr;
                    }
                    deallocate_memory_block(block_ptr);
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

    /// @brief Constructs object with given parameters on specified memory address.
    /// @tparam Type of the constructible object
    /// @tparam ...Args Pack of constructor arguments types
    /// @param ptr Pointer to the allocated memory for the object
    /// @param ...args Pack of constructor arguments
    template<typename U, typename ...Args>
    void construct(U* ptr, Args&&... args) {
        ::new((void*)ptr) U(std::forward<Args>(args)...);
    }

    /// @brief Destroys object on the specified address.
    /// @tparam U object type
    /// @param ptr pointer to the object
    template<typename U>
    void destroy(U* ptr) {
        ptr->~U();
    }

    /// @brief Returns the actual address of value.
    pointer address(reference value) const {
        return &value;
    }

    /// @brief Returns the actual address of value.
    const_pointer address(const_reference value) const {
        return &value;
    }

    /// @brief Deletes all of the allocated memory blocks.
    void clear() {
        auto block_ptr = blocks_list_head_;
        while (block_ptr != nullptr) {
            auto to_delete = block_ptr;
            block_ptr = block_ptr->next_ptr;
            deallocate_memory_block(to_delete);
        }
        size_ = 0;
        blocks_list_head_ = nullptr;
    }

    /// @brief Returns the number of allocated items.
    size_t size() const {
        return size_;
    }

    /// @brief Returns the number of allocated memory blocks.
    size_t blocks_allocated() const {
        return blocks_allocated_;
    }

    /// @brief Returns the maximum number of elements allowed for one allocation.
    size_t max_size() const {
        return block_size;
    }

private:
    /// @brief Sequental block of memory with fixed size and elements of type T.
    struct MemoryBlock {
        // marks non-empty cells
        BlockBitmap free_flags;
        // data in block 
        value_type buffer[block_size];
        // pointer to the next MemoryBlock in linked list
        MemoryBlock* next_ptr;

        pointer acquire_free_segment(size_t n) {
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

        void release_segment(pointer ptr, size_t n) {
            set_flags(ptr - buffer, n, false);
        }

        void set_flags(size_t from, size_t count, bool value) {
            for (size_t i = from; i < from + count; i++) {
                free_flags[i] = value;
            }
        }

        bool has_memory_segment(pointer ptr, size_t n) {
            auto buffer_ptr = static_cast<T*>(buffer);
            return ptr >= buffer_ptr && ptr + n <= buffer_ptr + block_size;
        }

        bool empty() {
            return free_flags.none();
        }
    };

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

    pointer find_free_segment(size_t n) {
        auto current_block_ptr = blocks_list_head_;
        while (current_block_ptr != nullptr) {
            if (auto ptr = current_block_ptr->acquire_free_segment(n); ptr != nullptr) {
                return ptr;
            }
            current_block_ptr = current_block_ptr->next_ptr;
        }
        return nullptr;
    }

    void swap(BlockAllocator&& other) {
        std::swap(size_, other.size_);
        std::swap(blocks_allocated_, other.blocks_allocated_);
        std::swap(blocks_list_head_, other.blocks_list_head_);
    }

    void copy(const BlockAllocator& other) {
        blocks_list_head_ = nullptr;
        MemoryBlock* other_ptr = other.blocks_list_head_;
        MemoryBlock* ptr = nullptr;
        while (other_ptr != nullptr) {
            auto block_copy_ptr = new MemoryBlock{*other_ptr};
            if (blocks_list_head_ == nullptr) {
                blocks_list_head_ = block_copy_ptr;
                ptr = block_copy_ptr;
            } else {
                ptr->next_ptr = block_copy_ptr;
                ptr = ptr->next_ptr;
            }
            other_ptr = other_ptr->next_ptr;
        }
        blocks_allocated_ = other.blocks_allocated_;
        size_ = other.size_;
    }

    size_t blocks_allocated_;
    size_t size_;
    MemoryBlock* blocks_list_head_;
};

// Allocator with zero-size blocks is not allowed.
template<typename T>
class BlockAllocator<T, 0> {};

} // namespace allocators
