#include "factory.hpp"

#include <exception>

#include <hash/default.hpp>
#include <hash/rs.hpp>


namespace bayan::hash {

HashProcessorFactory::HashProcessorFactory(const std::string& hash_type) 
    : hash_ptr_lazy_(nullptr) {
    if (hash_type != kHashTypeDefault &&
        hash_type != kHashTypeRs) {
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
    if (hash_type_ == kHashTypeDefault) {
        hash_ptr_lazy_ = std::make_shared<DefaultHashProcessor>();
    }
    if (hash_type_ == kHashTypeRs) {
        hash_ptr_lazy_ = std::make_shared<RsHashProcessor>();
    }
    return hash_ptr_lazy_;
}

void HashProcessorFactory::Swap(HashProcessorFactory&& other) {
    std::swap(hash_type_, other.hash_type_);
    std::swap(hash_ptr_lazy_, other.hash_ptr_lazy_);
}

} // namespace bayan::hash
