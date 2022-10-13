#include "parser.hpp"

#include <climits>
#include <sstream>
#include <vector>

#include <utils.hpp>

namespace parser {

ip::Address ParseAddress(const std::string& address_str) {
    const auto address_fields = utils::SplitString(address_str, '.');
    if (address_fields.size() != ip::kAddressSize) {
        throw std::runtime_error("invalid number of address bytes");
    }
    ip::Address address{};
    for (size_t i = 0; i < ip::kAddressSize; i++) {
        int byte_data{};
        try {
            byte_data = std::stoi(address_fields[i]);
        } catch (const std::logic_error&) {
            throw std::runtime_error("invalid address byte format");
        }
        if (byte_data > UCHAR_MAX) {
            throw std::runtime_error("address byte overflow");
        }
        address[i] = static_cast<ip::AddressByteT>(byte_data);
    }
    return address;
}

std::vector<ip::Address> ParseAddresses(std::istream& stream) {
    std::vector<ip::Address> result{};
    size_t line_counter = 0;
    for (std::string current_line{};
         std::getline(stream, current_line) && !current_line.empty();
         line_counter++) {
        const auto text_fields = utils::SplitString(current_line, '\t');
        ip::Address address{};
        try {
            address = ParseAddress(text_fields.at(0));
        } catch (const std::runtime_error& ex) {
            std::stringstream ss{};
            ss << "Invalid IP address representation " << '\'' << text_fields.at(0) << '\''
               << " on line " << line_counter << ": " << ex.what();
            throw std::runtime_error(ss.str());
        }
        result.push_back(std::move(address));
    }
    return result;
}

} // namespace parser
