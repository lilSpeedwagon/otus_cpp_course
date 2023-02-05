#pragma once

#include <hash/hash_processor.hpp>

namespace bayan::hash {

class DefaultHashProcessor : public HashProcessor {
public:
    Hash GetHash(const std::string& data) const override {
        return hasher_(data);
    }

private:
    std::hash<std::string> hasher_;
};
    
} // namespace bayan::hash
