#include "rs.hpp"


namespace bayan::hash {

Hash RsHashProcessor::GetHash(const std::string& data) const {
    Hash b = 378551;
    Hash a = 63689;
    Hash hash = 0;
    for (auto c : data) {
        hash = hash * a + static_cast<Hash>(c);
        a = a * b;
    }
    return hash;
}
    
} // namespace bayan::hash
