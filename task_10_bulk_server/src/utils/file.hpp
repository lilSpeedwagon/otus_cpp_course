#pragma once

#include <string>


namespace bulk_server::utils {

/// @brief Generates a randomized file name in format 
/// .\YYYY-MM-DDThh:mm:ss_XXXXXXXXXXXX" where X is a random digit
std::string GenerateFileName();

} // namespace bulk_server::utils
