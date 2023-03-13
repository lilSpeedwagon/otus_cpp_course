#pragma once

#include <string>


namespace join_server::handlers {

std::string HandleInsert(const std::string& table_name, const std::string& id, const std::string& value);

std::string HandleTruncate(const std::string& table_name);

std::string HandleIntersetion();

std::string HandleDifference();

} // namespace join_server::handlers
