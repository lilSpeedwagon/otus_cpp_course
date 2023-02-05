#pragma once

#include <memory>
#include <stdexcept>
#include <string>

#include <boost/noncopyable.hpp>

#include <hash/hash_processor.hpp>


namespace bayan::hash {

class HashProcessorFactory : private boost::noncopyable {
public:
    HashProcessorFactory(const std::string& hash_type);
    HashProcessorFactory(HashProcessorFactory&& other);
    ~HashProcessorFactory();

    HashProcessorFactory& operator=(HashProcessorFactory&& other);

    std::shared_ptr<HashProcessor> Create();

private:
    void Swap(HashProcessorFactory&& other);

    static constexpr const char* kHashTypeMd5 = "md5";
    static constexpr const char* kHashTypeDefault = "default";

    std::string hash_type_;
    std::shared_ptr<HashProcessor> hash_ptr_lazy_;
};

} // namespace bayan::hash
