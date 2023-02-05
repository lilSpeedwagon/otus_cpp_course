#pragma once

#include <hash/hash_processor.hpp>


namespace bayan::hash {

/// @brief Robert Sedgwick's simple hash algorithm implementation.
class RsHashProcessor : public HashProcessor {
public:
    Hash GetHash(const std::string& data) const override;
};
    
} // namespace bayan::hash
