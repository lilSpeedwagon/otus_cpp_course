#pragma once

#include <string>
#include <vector>

#include <ip_address.hpp>

namespace parser {

ip::Address ParseAddress(const std::string& address_str);

std::vector<ip::Address> ParseAddresses(std::istream& stream);

} // namespace parser
