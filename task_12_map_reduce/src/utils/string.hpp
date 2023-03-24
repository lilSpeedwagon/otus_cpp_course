#pragma once

#include <string>


namespace map_reduce::utils {

size_t MaxPrefixSize(const std::string& lhs, const std::string& rhs, size_t start_from = 0) {
    size_t i = start_from;
    for (; i < std::min(lhs.size(), rhs.size()) && lhs[i] == rhs[i]; i++) {}
    return i;
}

} // namespace map_reduce::utils
