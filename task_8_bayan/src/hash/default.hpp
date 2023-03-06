#pragma once

#include <hash/hash_processor.hpp>

namespace bayan::hash {

/// @brief Default STL implementation of hash processing
class DefaultHashProcessor : public HashProcessor {
public:
    /// @brief Calculate hash of the passed string
    /// @param data string representing hashed data
    /// @return hash of the data
    Hash GetHash(const std::string& data) const override;

private:
    std::hash<std::string> hasher_;
};
    
} // namespace bayan::hash
