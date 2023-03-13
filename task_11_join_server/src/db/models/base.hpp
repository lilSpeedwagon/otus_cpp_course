#pragma once

#include <list>
#include <string>


namespace join_server::db::models {

struct TableColumnSchema {
    std::string name;
    std::string type;
};

struct TableSchema {
    std::string name;
    std::list<TableColumnSchema> columns;
};

class BaseModel {};

} // namespace join_server::db::models
