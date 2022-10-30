#pragma once

#include <array>
#include <ostream>

namespace ip {

static constexpr const size_t kAddressSize = 4;
using AddressByteT = unsigned char;

struct Address : public std::array<AddressByteT, kAddressSize> {
public:
    Address() : std::array<AddressByteT, kAddressSize>{{0, 0, 0, 0}} {}
    Address(std::array<AddressByteT, kAddressSize>&& data) 
        : std::array<AddressByteT, kAddressSize>(std::move(data)) {}
    bool operator<(const Address& rhs) {
        return at(0) < rhs.at(0) && at(1) < rhs.at(1) &&
               at(2) < rhs.at(2) && at(3) < rhs.at(3);
    }
};

inline std::ostream& operator<<(std::ostream& stream, const Address& address) {
    for (size_t i = 0; i < kAddressSize - 1; i++) {
        stream << std::to_string(address[i]) << '.';
    }
    stream << std::to_string(address[kAddressSize - 1]);
    return stream;
}

} // namespace ip
