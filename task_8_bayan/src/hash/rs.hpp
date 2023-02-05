#pragma once

#include <hash/hash_processor.hpp>


namespace bayan::hash {

/// @brief Robert Sedgwick's simple hash algorithm implementation.
class RsHashProcessor : public HashProcessor {
public:
    /// @brief Calculate hash of the passed string
    /// @param data string representing hashed data
    /// @return hash of the data
    Hash GetHash(const std::string& data) const override;
};
    
} // namespace bayan::hash
