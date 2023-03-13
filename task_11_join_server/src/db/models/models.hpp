#pragma once

#include <string>

#include <db/models/base.hpp>


namespace join_server::db::models {

static const ColumnsMeta kColumnsMetadata = {
    {"id", "int"},
    {"name", "varchar"}
};

static const auto kAMetadata = TableSchema{
    "A",                // name
    kColumnsMetadata,   // columns
};

static const auto kBMetadata = TableSchema{
    "B",                // name
    kColumnsMetadata,   // columns
};

struct A : public BaseModel {
    int id;
    std::string name;
};

struct B : public BaseModel {
    int id;
    std::string name;
};

} // namespace join_server::db::models
