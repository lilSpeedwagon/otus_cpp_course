#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <ip_address.hpp>
#include <parser.hpp>

int main() {
    auto addresses = parser::ParseAddresses(std::cin);

    // sort with descending order
    std::sort(addresses.begin(), addresses.end(),
              [](const auto& lhs, const auto& rhs) { return lhs > rhs; });

    // 1. print sorted
    for (const auto& address : addresses) {
        std::cout << address << '\n';
    }

    // 2. print sorted and 1st byte is 1
    for (const auto& address : addresses) {
        if (address[0] == 1) {
            std::cout << address << '\n';
        }
    }

    // 3. print sorted, 1st byte is 46, 2nd is 70
    for (const auto& address : addresses) {
        if (address[0] == 46 && address[1] == 70) {
            std::cout << address << '\n';
        }
    }

    // 4. print sorted and any byte is 46
    for (const auto& address : addresses) {
        for (size_t i = 0; i < ip::kAddressSize; i++) {
            if (address[i] == 46) {
                std::cout << address << '\n';
            }
        }
    }

    return 0;
}
