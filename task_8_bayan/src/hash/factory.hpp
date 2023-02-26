#pragma once

#include <memory>
#include <stdexcept>

#include <boost/noncopyable.hpp>

#include <hash/hash_processor.hpp>


namespace bayan::hash {

/// @brief Abstract hash processor factory. Builds HashProcessor based
/// on string representation of the hash algorythm. Based on lazy loading
/// and reuses the same HashProcessor via smart pointer. Noncopyable. 
class HashProcessorFactory : private boost::noncopyable {
public:
    /// @brief Constructor
    /// @param hash_type hash algorithm title
    HashProcessorFactory(const std::string& hash_type);
    HashProcessorFactory(HashProcessorFactory&& other);
    ~HashProcessorFactory();

    HashProcessorFactory& operator=(HashProcessorFactory&& other);

    /// @brief Create an instance of HashProcessor
    /// @return Smart pointer to the created HashProcessor
    std::shared_ptr<HashProcessor> Create();

private:
    void Swap(HashProcessorFactory&& other);

    static constexpr const char* kHashTypeDefault = "default";
    static constexpr const char* kHashTypeRs = "rs";

    std::string hash_type_;
    std::shared_ptr<HashProcessor> hash_ptr_lazy_;
};

} // namespace bayan::hash
