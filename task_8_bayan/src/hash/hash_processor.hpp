#pragma once

#include <string>

#include <hash/hash.hpp>


namespace bayan::hash {

class HashProcessor {
public:
    HashProcessor() {}
    virtual ~HashProcessor() {}

    virtual Hash Hash(const std::string& data) const = 0;
};

} // namespace bayan::hash
