#pragma once

#include <iostream>
#include <list>
#include <tuple>
#include <vector>

namespace ip {

namespace impl {

template<typename>
struct is_tuple : public std::false_type {};

template<typename ...T>
struct is_tuple<std::tuple<T...>> : public std::true_type {};

template<int I = 0, typename ...Args>
void print_tuple(const std::tuple<Args...>& tuple, std::ostream& stream) {
    if constexpr (I < std::tuple_size_v<std::tuple<Args...>>) {
        auto& value = std::get<I>(tuple);
        static_assert(std::is_same<decltype(std::get<0>(tuple)), decltype(value)>::value,
                      "tuple element types must be the same");
        if constexpr (I != 0) {
            stream << '.';
        }
        stream << value;
        print_tuple<I + 1, Args...>(tuple, stream);
    }
}

} // namespace impl

/// @brief Prints IP address with custom number of elements.
/// Template specialization for tuple with elements of the same type.
/// @tparam T tuple type
/// @param tuple tuple to print as an IP address
template<typename T,
         typename std::enable_if<impl::is_tuple<T>::value, bool>::type = true>
void print_ip(const T& tuple, std::ostream& stream = std::cout) {
    impl::print_tuple(tuple, stream);
}

/// @brief Prints IP address with custom number of elements.
/// Template specialization for integral types. Every byte of the integer is treated as a IP address element.
/// @tparam T integral type
/// @param address integer to print as an IP address
template<typename T,
         typename std::enable_if<std::is_integral<T>::value, bool>::type = true>
void print_ip(const T& address, std::ostream& stream = std::cout) {
    const size_t kByteSize = 8;
    bool first = true;
    for (int i = sizeof(T) - 1; i >= 0; i--) {
        if (first) {
            first = false;
        } else {
            stream << '.';
        }
        stream << std::to_string(static_cast<uint8_t>(address >> i * kByteSize));
    }
}

/// @brief Prints IP address with custom number of elements.
/// Template specialization for std::string. Address is printed as is.
/// @param address string to print as an IP address
template<typename T,
         typename std::enable_if<std::is_same<T, std::string>::value, bool>::type = true>
void print_ip(const T& address, std::ostream& stream = std::cout) {
    stream << address;
}

/// @brief Prints IP address with custom number of elements.
/// Template specialization for containers. Each container item is printed as is.
/// @tparam T container type
/// @tparam U item type
/// @param address container to print as an IP address
template<typename T, typename U = typename T::value_type,
         typename std::enable_if<std::is_same<T, std::vector<U>>::value ||
                                 std::is_same<T, std::list<U>>::value, bool>::type = true>
void print_ip(const T& address, std::ostream& stream = std::cout) {
    bool first = true;
    for (const auto& value : address) {
        if (first) {
            first = false;
        } else {
            stream << '.';
        }
        stream << value;
    }
}

} // namespace ip
