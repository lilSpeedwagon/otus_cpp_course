#include "file.hpp"

#include <random>
#include <sstream>

#include <utils/time.hpp>


namespace map_reduce::utils {

std::string GenerateFileName() {
    static std::random_device rd;
    static const size_t kRandomStringSize = 12;

    std::stringstream ss;
    ss << "./" << GetTimeString() << '_';
    std::uniform_int_distribution<char> dist(0, 9);
    for (size_t i = 0; i < kRandomStringSize; i++) {
        ss << std::to_string(dist(rd));
    }
    return ss.str();
}

} // namespace map_reduce::utils
