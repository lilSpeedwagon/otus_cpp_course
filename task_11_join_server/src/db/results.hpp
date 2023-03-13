#pragma once

#include <optional>
#include <string>
#include <vector>


namespace join_server::db {

struct Column {
    std::string name;
    std::optional<std::string> value;
};

using Row = std::vector<Column>;

} // namespace join_server::db
