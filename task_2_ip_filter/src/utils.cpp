#include "utils.hpp"

#include <algorithm>

namespace utils {

std::vector<std::string> SplitString(const std::string& str, char delimiter) {
    std::vector<std::string> result{};
    auto begin_it = str.begin();
    auto end_it = begin_it;
    for (;;) {
        end_it = std::find(end_it, str.end(), delimiter);
        result.emplace_back(begin_it, end_it);
        if (end_it == str.end()) {
            break;
        }
        begin_it = ++end_it;
    }
    return result;
}

} // namespace utils
