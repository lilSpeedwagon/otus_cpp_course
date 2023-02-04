#pragma once

#include <stdint.h>
#include <vector>


namespace bayan::hash {

using Hash = unsigned long;

Hash HashCombine(Hash lhs, Hash rhs) {
    lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
    return lhs;
}

} // namespace bayan::hash
