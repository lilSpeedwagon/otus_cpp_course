#pragma once

#include <array>

#include <boost/functional/hash/hash.hpp>


namespace std {

template<size_t N>
struct hash<std::array<size_t, N>> {
    std::size_t operator()(const std::array<size_t, N>& arr) const {
        std::size_t result = 0;
        for (const auto& item : arr) {
            boost::hash_combine(result, item);
        }
        return result;
    }
};

} // namespace std

namespace containers {

template<size_t Size>
std::array<size_t, Size + 1> AppendToIndex(const std::array<size_t, Size>& index, size_t value) {
    std::array<size_t, Size + 1> new_index;
    for (size_t i = 0; i < index.size(); i++) {
        new_index[i] = index[i];
    }
    new_index[new_index.size() - 1] = value;
    return new_index;
}

std::array<size_t, 1> AppendToIndex(size_t value) {
    return std::array<size_t, 1>{value};
}

} // namespace containers
