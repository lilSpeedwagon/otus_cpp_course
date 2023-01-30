#pragma once

#include <hash/hash_processor.hpp>


namespace bayan::hash {

class Md5HashProcessor : public HashProcessor {
public:
    Md5HashProcessor() {}
    virtual ~Md5HashProcessor() {}

    size_t Hash(const std::string& data) const override {
        return 0;
    }
};

} // namespace bayan::hash
