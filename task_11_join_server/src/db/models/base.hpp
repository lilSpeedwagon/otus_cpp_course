#pragma once

#include <list>
#include <string>
#include <vector>


namespace join_server::db::models {

/// @brief SQL table column metadata
struct TableColumnSchema {
    std::string name;
    std::string type;
};

using ColumnsMeta = std::vector<TableColumnSchema>;

/// @brief SQL table metadata
struct TableSchema {
    std::string name;
    ColumnsMeta columns;
};

} // namespace join_server::db::models
