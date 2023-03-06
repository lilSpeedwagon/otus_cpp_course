#pragma once

#include <string>


namespace async::utils {

/// @brief Generates a randomized file name in format 
/// .\YYYY-MM-DDThh:mm:ss_XXXXXXXXXXXX" where X is a random digit
std::string GenerateFileName();

} // namespace async::utils
