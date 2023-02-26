#include "default.hpp"


namespace bayan::hash {

Hash DefaultHashProcessor::GetHash(const std::string& data) const {
    return hasher_(data);
}
    
} // namespace bayan::hash
