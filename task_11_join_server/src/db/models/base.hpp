#pragma once

#include <list>
#include <string>


namespace join_server::db::models {

struct TableColumnSchema {
    std::string name;
    std::string type;
};

using ColumnsMeta = std::list<TableColumnSchema>;

struct TableSchema {
    std::string name;
    ColumnsMeta columns;
};

class BaseModel {};

} // namespace join_server::db::models
