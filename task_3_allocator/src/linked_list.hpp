#pragma once

#include <iterator>

namespace containers {

/// @brief A single node holding value of LinkedList<T>.
template<typename T>
struct ListNode {
    T value;
    ListNode* next_ptr;
};

/// @brief Simple one-directional linked list.
/// @tparam T type of the stored values
/// @tparam AllocatorT type of the allocator, by default - std::allocator
template<typename T, typename AllocatorT = std::allocator<ListNode<T>>>
class LinkedList {
public:
    using value_type = T;
    using reference_type = T&;
    using node_type = ListNode<T>;

    /// @brief Forward iterator to the stored items.
    class iterator : public std::iterator<std::forward_iterator_tag, T> {
    public:
        explicit iterator(node_type* node) : ptr_{node} {}
        iterator& operator++() {
            if (ptr_ != nullptr) {
                ptr_ = ptr_->next_ptr;
            }
            return *this;
        }
        iterator operator++(int) {
            auto result = *this;
            ++(*this);
            return result;
        }
        bool operator==(iterator other) const {
            return ptr_ == other.ptr_;
        }
        bool operator!=(iterator other) const {
            return !(*this == other);
        }
        reference_type operator*() const {
            return ptr_->value;
        }

    private:
        node_type* ptr_;
    };

    LinkedList() : head_ptr_{nullptr}, size_{0} {}
    LinkedList(const LinkedList& other) {
        clear();
        copy(other);
    }
    LinkedList& operator=(const LinkedList& other) {
        clear();
        copy(other);
        return *this;
    }
    LinkedList(LinkedList&& other) : head_ptr_{nullptr}, size_{0} {
        swap(std::move(other));
    }
    LinkedList& operator=(LinkedList&& other) {
        swap(std::move(other));
        return *this;
    }
    ~LinkedList() {
        clear();
    }

    /// @brief Add a value to the head of the list.
    /// @param value value to add
    void push_front(const value_type& value) {
        auto ptr = allocator_.allocate(1);
        ptr->value = value;
        ptr->next_ptr = head_ptr_;
        head_ptr_ = ptr;
        size_++;
    }

    /// @brief Delete all of the stored values.
    void clear() {
        auto ptr = head_ptr_;
        while (ptr != nullptr) {
            auto to_delete_ptr = ptr;
            ptr = ptr->next_ptr;
            allocator_.destroy(to_delete_ptr);
            allocator_.deallocate(to_delete_ptr, 1);
        }
        size_ = 0;
        head_ptr_ = nullptr;
    }

    /// @brief Returns a number of the stored elements. 
    size_t size() const {
        return size_;
    }

    /// @brief Returns true if list is empty. 
    bool empty() const {
        return size_ == 0;
    }

    /// @brief Returns forward iterator to the last stored value.
    iterator begin() const {
        return iterator(head_ptr_);
    }

    /// @brief Return iterator marking the end of the list.
    iterator end() const {
        return iterator(nullptr);
    }

private:
    void copy(const LinkedList& other) {
        head_ptr_ = nullptr;
        node_type* other_ptr = other.head_ptr_;
        node_type* ptr = nullptr;
        while (other_ptr != nullptr) {
            auto node_copy_ptr = allocator_.allocate(1);
            allocator_.construct(node_copy_ptr, *other_ptr);
            if (head_ptr_ == nullptr) {
                head_ptr_ = node_copy_ptr;
                ptr = node_copy_ptr;
            } else {
                ptr->next_ptr = node_copy_ptr;
                ptr = ptr->next_ptr;
            }
            other_ptr = other_ptr->next_ptr;
        }
        size_ = other.size_;
    }

    void swap(LinkedList&& other) {
        std::swap(head_ptr_, other.head_ptr_);
        std::swap(size_, other.size_);
        std::swap(allocator_, other.allocator_);
    }

    node_type* head_ptr_;
    size_t size_;
    AllocatorT allocator_;
};

} // namespace containers
