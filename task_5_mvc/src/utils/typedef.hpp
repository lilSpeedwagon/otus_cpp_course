#pragma once

#include <utility>

namespace utils {

/// @class Strong typedef implementation.
/// @param T underlying type
/// @param Tag any type to differ typedefs with the same underlying type
template<typename T, typename Tag>
class StrongTypedef {
public:
    StrongTypedef() : value_() {}
    explicit StrongTypedef(const T& value) : value_(value) {}
    explicit StrongTypedef(T&& value) : value_(std::move(value)) {}
    StrongTypedef(const StrongTypedef& other) {
        value_ = other.value_;
    }
    StrongTypedef(StrongTypedef&& other) {
        std::swap(value_, other.value_);
    }
    ~StrongTypedef() {}

    StrongTypedef& operator=(const T& value) {
        value_ = value;
        return *this;
    }
    StrongTypedef& operator=(T&& value) 
        noexcept(std::is_nothrow_move_constructible<T>::value) {
        value_ = std::move(value);
        return *this;
    }
    StrongTypedef& operator=(const StrongTypedef& other) {
        value_ = other.value_;
        return *this;
    }
    StrongTypedef& operator=(StrongTypedef&& other) 
        noexcept(std::is_nothrow_move_constructible<T>::value) {
        std::swap(value_, other.value_);
        return *this;
    }
    bool operator==(const StrongTypedef& other) const {
        return value_ == other.value_;
    }
    bool operator<(const StrongTypedef& other) const {
        return value_ < other.value_;
    }

    /// @brief Returns a ref to the underlying value.
    T& GetUnderlying() noexcept {
        return value_;
    }

    /// @brief Returns a const ref to the underlying value.
    const T& GetUnderlying() const noexcept {
        return value_;
    }

private:
    T value_;
};

} // namespace utils

namespace std {

template<typename T, typename Tag>
struct hash<utils::StrongTypedef<T, Tag> > {
    std::size_t operator()(const utils::StrongTypedef<T, Tag>& t) const {
        return std::hash<T>{}(t.GetUnderlying());
    }
};

} // namepspace std
