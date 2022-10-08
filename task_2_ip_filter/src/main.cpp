#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "ip_address.hpp"


std::vector<std::string> SplitString(const std::string& str, char delimiter) {
    std::vector<std::string> result{};
    auto begin_it = str.begin();
    auto end_it = begin_it;
    for (;;) {
        end_it = std::find(end_it, str.end(), delimiter);
        result.emplace_back(begin_it, end_it);
        if (end_it == str.end()) {
            break;
        }
        begin_it = ++end_it;
    }
    return result;
}

std::vector<ip::Address> ParseAddresses() {
    std::vector<ip::Address> result{};
    for (std::string current_line{}; std::getline(std::cin, current_line) && !current_line.empty();) {
        auto text_fields = SplitString(current_line, '\t');
        auto address_fields = SplitString(text_fields.at(0), '.');
        if (address_fields.size() != ip::kAddressSize) {
            throw std::runtime_error("Invalid IP address representation");
        }
        ip::Address address{};
        for (size_t i = 0; i < ip::kAddressSize; i++) {
            address[i] = static_cast<ip::AddressByteT>(std::stoi(address_fields[i]));
        }
        result.push_back(std::move(address));
    }
    return result;
}

int main() {
    auto addresses = ParseAddresses();

    // sort descending with order
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
