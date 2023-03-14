#pragma once

#include <optional>
#include <string>
#include <vector>


namespace join_server::db {

/// @brief A single column of SQL resulting row 
struct Column {
    std::string name;
    std::optional<std::string> value;
};

using Row = std::vector<Column>;

} // namespace join_server::db
