#pragma once

#include <chrono>
#include <string>


namespace bulk_server::utils {

/// @brief Converts timepoint to string representation according to the current locale.
/// @param tp timepoint to convert
/// @return string representing the timepoint
std::string GetTimeString(const std::chrono::system_clock::time_point& tp);

/// @brief Converts the current time to string representation according to the current locale.
/// @return string representing the current time
std::string GetTimeString();

} // namespace bulk_server::utils
