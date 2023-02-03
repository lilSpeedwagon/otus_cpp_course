#pragma once
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"  // allow usage of MD5

#include <cstring>

#include <openssl/md5.h>

#include <hash/hash.hpp>
#include <hash/hash_processor.hpp>


namespace bayan::hash {

class Md5HashProcessor : public HashProcessor {
public:
    Md5HashProcessor() {}
    virtual ~Md5HashProcessor() {}

    hash::Hash GetHash(const std::string& data) const override {
        constexpr size_t hash_size = MD5_DIGEST_LENGTH;
        unsigned char buffer[hash_size];
        MD5(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), buffer);

        hash::Hash result;
        result.reserve(hash_size);
        for (size_t i = 0; i < hash_size; i++) {
            result.push_back(buffer[i]);
        }
        return result;
    }
};

} // namespace bayan::hash
