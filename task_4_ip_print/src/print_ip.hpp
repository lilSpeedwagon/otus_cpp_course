#pragma once

#include <iostream>
#include <list>
#include <tuple>
#include <vector>

/// @brief ip address utility functions
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
/// Has following specializations:
/// - std::tuple<Args...> - prints elements as separate address bytes. All types in Args... must be the same
/// - std::string - prints element address as is
/// - integral types - prints each byte of an integral type as a separate address byte
/// - sequental container types (std::vector<T>, std::list<T>) - prints each element as a separate address byte
/// @tparam T address type
/// @param address address to print
template<typename T,
         typename std::enable_if<impl::is_tuple<T>::value, bool>::type = true>
void print_ip(const T& address, std::ostream& stream = std::cout) {
    impl::print_tuple(address, stream);
}

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

typename std::enable_if<std::is_same<T, std::string>::value, bool>::type = true>
void print_ip(const T& address, std::ostream& stream = std::cout) {
    stream << address;
}

template<typename T,
         typename std::enable_if<std::is_same<T, std::vector<typename T::value_type>>::value ||
                                 std::is_same<T, std::list<typename T::value_type>>::value, bool>::type = true>
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
