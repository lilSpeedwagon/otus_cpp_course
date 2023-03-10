#include "time.hpp"

#include <stdexcept>
#include <time.h>

namespace join_server::utils {

std::string GetTimeString(const std::chrono::system_clock::time_point& tp) {
    constexpr const char* kFormat = "%FT%T";  // ISO-8601 without timezone
    
    auto raw_time = std::chrono::system_clock::to_time_t(tp);
    tm* time_data_ptr = std::gmtime(&raw_time);
    if (time_data_ptr == nullptr) {
        throw std::runtime_error("Bad timepoint convertation");
    }
    constexpr size_t kBufferSize = 256;
    char buffer[kBufferSize];
    const auto result = strftime(buffer, kBufferSize, kFormat, time_data_ptr);
    if (!result) {
        throw std::runtime_error("Bad timepoint format");
    }
    return std::string(buffer);
}

std::string GetTimeString() {
    return GetTimeString(std::chrono::system_clock::now());
}

} // namespace join_server::utils
