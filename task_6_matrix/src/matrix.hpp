#pragma once

#include <unordered_map>

#include <index.hpp>


namespace containers {

template<typename T, T DefaultValue, size_t Degree = 2>
class InfiniteMatrix {
public:

    static_assert(Degree >= 2, "InfiniteMatrix degree lesser than 2 is not supported");

    using index_type = std::array<size_t, Degree>;
    using container_type = std::unordered_map<index_type, T>;
    
    template<size_t IndexSize, size_t DegreeIndex>
    class ProxyIterator {
    public:
        ProxyIterator(std::shared_ptr<container_type> matrix_ptr,
                      const std::array<size_t, IndexSize>& index) 
            : index_(index), matrix_ptr_(matrix_ptr) {}

        ProxyIterator<IndexSize + 1, DegreeIndex - 1> operator[](size_t idx) {
            if (matrix_ptr_.expired()) {
                throw std::runtime_error("bad iterator");
            }
            const auto new_index = AppendToIndex(index_, idx);
            return ProxyIterator<IndexSize + 1, DegreeIndex - 1>(matrix_ptr_, new_index);
        }

    private:
        std::array<size_t, IndexSize> index_;
        std::weak_ptr<container_type> matrix_ptr_;
    };

    template<size_t IndexSize>
    class ProxyIterator<IndexSize, 1> {
    public:
        ProxyIterator(std::shared_ptr<container_type> matrix_ptr,
                      const std::array<size_t, IndexSize>& index) 
            : index_(index), matrix_ptr_(matrix_ptr) {}

        const T& operator[](size_t idx) const {
            if (matrix_ptr_.expired()) {
                throw std::runtime_error("bad iterator");
            }
            const auto new_index = AppendToIndex(index_, idx);
            if (auto it = matrix_ptr_->find(new_index);
                it != matrix_ptr_->end()) {
                return it->second;
            }
            return DefaultValue;
        }

        T& operator[](size_t idx) {
            if (matrix_ptr_.expired()) {
                throw std::runtime_error("bad iterator");
            }
            const auto new_index = AppendToIndex(index_, idx);
            auto ptr = matrix_ptr_.lock();
            if (auto it = ptr->find(new_index);
                it != ptr->end()) {
                return it->second;
            }
            auto [inserted_it, _] = ptr->insert(std::make_pair(new_index, DefaultValue));
            return inserted_it->second;
        }

    private:
        std::array<size_t, IndexSize> index_;
        std::weak_ptr<container_type> matrix_ptr_;
    };


    InfiniteMatrix() {
        matrix_ptr_ = std::make_shared<container_type>();
    }
    InfiniteMatrix(const InfiniteMatrix& other) {
        matrix_ptr_ = std::make_shared(*other.matrix_ptr_);
    }
    InfiniteMatrix(InfiniteMatrix&& other) {
        std::swap(matrix_ptr_, other.matrix_ptr_);
    }
    ~InfiniteMatrix() {}

    InfiniteMatrix& operator=(const InfiniteMatrix& other) {
        matrix_ptr_ = std::make_shared(*other.matrix_ptr_);
        return *this;
    }
    InfiniteMatrix& operator=(InfiniteMatrix&& other) {
        std::swap(matrix_ptr_, other.matrix_ptr_);
        return *this;
    }

    size_t size() const {
        return matrix_ptr_->size();
    }

    bool empty() const {
        return matrix_ptr_->empty();
    }

    ProxyIterator<1, Degree - 1>  operator[](size_t idx) {
        std::array<size_t, 1> index{idx};
        return ProxyIterator<1, Degree - 1>(matrix_ptr_, index);
    }

    typename container_type::iterator begin() const {
        return matrix_ptr_->begin();
    }

    typename container_type::iterator end() const {
        return matrix_ptr_->end();
    }

private:

    std::shared_ptr<container_type> matrix_ptr_;
};

} // namespace containers
