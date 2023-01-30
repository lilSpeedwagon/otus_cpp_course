#include "factory.hpp"

#include <exception>

#include <hash/md5.hpp>


namespace bayan::hash {

HashProcessorFactory::HashProcessorFactory(const std::string& hash_type) 
    : hash_ptr_lazy_(nullptr) {
    if (hash_type != kHashTypeMd5) {
        throw std::runtime_error("unknown hash type");
    }
    hash_type_ = hash_type;
}

HashProcessorFactory::HashProcessorFactory(HashProcessorFactory&& other) {
    Swap(std::move(other));
}

HashProcessorFactory::~HashProcessorFactory() {}

HashProcessorFactory& HashProcessorFactory::operator=(HashProcessorFactory&& other) {
    Swap(std::move(other));
    return *this;
}

std::shared_ptr<HashProcessor> HashProcessorFactory::Create() {
    if (hash_ptr_lazy_ != nullptr) {
        return hash_ptr_lazy_;
    }
    if (hash_type_ == kHashTypeMd5) {
        hash_ptr_lazy_ = std::make_shared<Md5HashProcessor>();
    }
    return hash_ptr_lazy_;
}

void HashProcessorFactory::Swap(HashProcessorFactory&& other) {
    std::swap(hash_type_, other.hash_type_);
    std::swap(hash_ptr_lazy_, other.hash_ptr_lazy_);
}

} // namespace bayan::hash
