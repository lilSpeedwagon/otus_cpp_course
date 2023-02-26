#pragma once

#include <string>

#include <hash/hash.hpp>


namespace bayan::hash {

/// @brief Hash processor interface
class HashProcessor {
public:
    HashProcessor() = default;
    virtual ~HashProcessor() = default;

    /// @brief Calculate hash of the passed string
    /// @param data string representing hashed data
    /// @return hash of the data
    virtual Hash GetHash(const std::string& data) const = 0;
};

} // namespace bayan::hash
