#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "ip_address.hpp"


std::string BuildInvalidAddressMessage(const std::string& address_str, size_t line_num) {
    std::stringstream ss{};
    ss << "Invalid IP address representation "
       << '\'' << address_str << '\''
       << " on line " << line_num;
    return ss.str();
}

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
    size_t line_counter = 0;
    for (std::string current_line{};
         std::getline(std::cin, current_line) && !current_line.empty();
         line_counter++) {
        const auto text_fields = SplitString(current_line, '\t');
        const auto& address_str = text_fields.at(0);
        const auto address_fields = SplitString(address_str, '.');
        if (address_fields.size() != ip::kAddressSize) {
            throw std::runtime_error(BuildInvalidAddressMessage(address_str, line_counter));
        }
        ip::Address address{};
        for (size_t i = 0; i < ip::kAddressSize; i++) {
            try {
                address[i] = static_cast<ip::AddressByteT>(std::stoi(address_fields[i]));
            } catch (const std::logic_error& ex) {
                throw std::runtime_error(BuildInvalidAddressMessage(address_str, line_counter));
            }
        }
        result.push_back(std::move(address));
    }
    return result;
}

int main() {
    auto addresses = ParseAddresses();

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
