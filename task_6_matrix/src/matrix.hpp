#pragma once

#include <unordered_map>

#include <index.hpp>


namespace containers {

/// @brief Infinite matrix filled with the default values.
/// Only non-default values are physically stored in memory. 
/// @tparam T type of the stored elements 
/// @tparam DefaultValue default value of the elements
/// @tparam Dimensions number of matrix dimensions
template<typename T, T DefaultValue, size_t Dimensions = 2>
class InfiniteMatrix {
public:
    static_assert(Dimensions >= 2, "InfiniteMatrix degree lesser than 2 is not supported");

    // forward declaration
    class ValueProxy;
    
    /// @brief Type of matrix index.
    using index_type = std::array<size_t, Dimensions>;
    /// @brief Type of internal container storing non-default elements
    using container_type = std::unordered_map<index_type, ValueProxy>;

    /// @brief Proxy class for storing a single matrix element value 
    /// along with it's index in the matrix. Has implicit cast operator 
    /// to the underlying element type. The proxy is used to set/unset
    /// values in the internal container on every value change.
    /// If some value is set to DefaultValue the corresponding value
    /// in the container is erased.
    class ValueProxy {
    public:
        /// @brief Explicit constructor
        /// @param matrix_ptr a weak pointer to the matrix internal container
        /// @param index index of the element
        /// @param value value of the element
        explicit ValueProxy(std::weak_ptr<container_type> matrix_ptr,
                   std::array<size_t, Dimensions>&& index, T value) 
            : matrix_ptr_(matrix_ptr), index_(std::move(index)), value_(value) {}
        /// @brief Copy-constructor
        ValueProxy(const ValueProxy& other) {
            Copy(other);
        }
        /// @brief Move-constructor
        ValueProxy(ValueProxy&& other) {
            Swap(std::move(other));
        }
        /// @brief Assign-copy operator
        ValueProxy& operator=(const ValueProxy& other) {
            Copy(other);
            return *this;
        }
        /// @brief Assign-move operator
        ValueProxy& operator=(ValueProxy&& other) {
            Swap(std::move(other));
            return *this;
        }

        /// @brief Assign operator for the stored value.
        /// The corresponding value is also updated in the matrix container.
        /// Element is erased if value is set to DefaultValue.
        /// @param value new value
        /// @return *this
        ValueProxy& operator=(const T& value) {
            if (matrix_ptr_.expired()) {
                throw std::runtime_error("bad iterator");
            }
            value_ = value;
            if (value == DefaultValue) {
                matrix_ptr_.lock()->erase(index_);
            } else {
                matrix_ptr_.lock()->insert_or_assign(index_, *this);
            }
            return *this;
        }

        /// @brief Implicit cast operator to the underlying value.
        operator T() const {
            return value_;
        }

    private:
        void Swap(ValueProxy&& other) {
            std::swap(matrix_ptr_, other.matrix_ptr_);
            std::swap(index_, other.index_);
            std::swap(value_, other.value_);
        }
        void Copy(const ValueProxy& other) {
            matrix_ptr_ = other.matrix_ptr_;
            index_ = other.index_;
            value_ = other.value_;
        }

        std::weak_ptr<container_type> matrix_ptr_;
        std::array<size_t, Dimensions> index_;
        T value_;
    };
    
    /// @brief Proxy iterator for the non-terminal dimension of the matrix.
    /// Intended for accessing matrix elements through several dimensions.
    /// @tparam IndexSize number of known index dimensions
    /// @tparam DimensionIndex number of the dimensions until the end elements
    template<size_t IndexSize, size_t DimensionIndex>
    class IteratorProxy {
    public:
        /// @brief Explicit constructor
        /// @param matrix_ptr a weak pointer to the matrix elements container
        /// @param index the known index accessed before
        explicit IteratorProxy(std::weak_ptr<container_type> matrix_ptr,
                               const std::array<size_t, IndexSize>& index) 
            : index_(index), matrix_ptr_(matrix_ptr) {}

        /// @brief Index access operator to reach the underlying dimension proxies/values.
        /// @param idx index of the element in the current matrix dimension
        /// @return underlying dimension IteratorProxy
        IteratorProxy<IndexSize + 1, DimensionIndex - 1> operator[](size_t idx) {
            if (matrix_ptr_.expired()) {
                throw std::runtime_error("bad iterator");
            }
            const auto new_index = AppendToIndex(index_, idx);
            return IteratorProxy<IndexSize + 1, DimensionIndex - 1>(matrix_ptr_, new_index);
        }

    private:
        std::array<size_t, IndexSize> index_;
        std::weak_ptr<container_type> matrix_ptr_;
    };

    /// @brief Proxy iterator template specialisation for the terminal dimension of the matrix.
    /// @tparam IndexSize number of known index dimensions
    template<size_t IndexSize>
    class IteratorProxy<IndexSize, 1> {
    public:
        /// @brief Explicit constructor
        /// @param matrix_ptr a weak pointer to the matrix elements container
        /// @param index the known index accessed before
        explicit IteratorProxy(std::weak_ptr<container_type> matrix_ptr,
                               const std::array<size_t, IndexSize>& index) 
            : index_(index), matrix_ptr_(matrix_ptr) {}

        /// @brief Index access const operator to reach underlying values.
        /// @param idx index of the element in the terminating dimension of the matrix
        /// @return ValueProxy of the element
        const ValueProxy operator[](size_t idx) const {
            if (matrix_ptr_.expired()) {
                throw std::runtime_error("bad iterator");
            }
            const auto new_index = AppendToIndex(index_, idx);
            if (auto it = matrix_ptr_->find(new_index);
                it != matrix_ptr_->end()) {
                return it->second;
            }
            return ValueProxy(matrix_ptr_, new_index, DefaultValue);
        }

        /// @brief Index access operator to reach underlying values.
        /// @param idx index of the element in the terminating dimension of the matrix
        /// @return ValueProxy of the element
        ValueProxy operator[](size_t idx) {
            if (matrix_ptr_.expired()) {
                throw std::runtime_error("bad iterator");
            }
            auto new_index = AppendToIndex(index_, idx);
            auto ptr = matrix_ptr_.lock();
            if (auto it = ptr->find(new_index);
                it != ptr->end()) {
                return it->second;
            }
            return ValueProxy(matrix_ptr_, std::move(new_index), DefaultValue);
        }

    private:
        std::array<size_t, IndexSize> index_;
        std::weak_ptr<container_type> matrix_ptr_;
    };

    /// @brief Default constructor
    InfiniteMatrix() {
        matrix_ptr_ = std::make_shared<container_type>();
    }
    /// @brief Copy constructor
    InfiniteMatrix(const InfiniteMatrix& other) {
        matrix_ptr_ = std::make_shared(*other.matrix_ptr_);
    }
    /// @brief move constructor
    InfiniteMatrix(InfiniteMatrix&& other) {
        std::swap(matrix_ptr_, other.matrix_ptr_);
    }
    ~InfiniteMatrix() {}

    /// @brief Copy-assign operator
    InfiniteMatrix& operator=(const InfiniteMatrix& other) {
        matrix_ptr_ = std::make_shared(*other.matrix_ptr_);
        return *this;
    }
    /// @brief Move-assign operator
    InfiniteMatrix& operator=(InfiniteMatrix&& other) {
        std::swap(matrix_ptr_, other.matrix_ptr_);
        return *this;
    }

    /// @brief Returns the number of stored non-default values.
    size_t size() const {
        return matrix_ptr_->size();
    }

    /// @brief Returns true if matrix contains some non-default values.
    bool empty() const {
        return matrix_ptr_->empty();
    }

    /// @brief Index access operator.
    /// @param idx index of the underlying dimension
    /// @return Iteratorproxy for the underlying matrix dimension
    IteratorProxy<1, Dimensions - 1> operator[](size_t idx) {
        std::array<size_t, 1> index{idx};
        return IteratorProxy<1, Dimensions - 1>(matrix_ptr_, index);
    }

    /// @brief Iterator to the beginning of the stored elements sequence.
    typename container_type::iterator begin() const {
        return matrix_ptr_->begin();
    }

    /// @brief Iterator to the end of the stored elements sequence.
    typename container_type::iterator end() const {
        return matrix_ptr_->end();
    }

private:
    std::shared_ptr<container_type> matrix_ptr_;
};

} // namespace containers
