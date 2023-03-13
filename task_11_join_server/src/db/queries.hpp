#pragma once

#include <string>
#include <sstream>

#include <db/models/base.hpp>


namespace join_server::db::queries {

std::string CreateTableQuery(const models::TableSchema& meta) {
    std::stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS " << meta.name << " (";
    bool is_first = true;
    for (const auto& c : meta.columns) {
        if (is_first) {
            is_first = false;
        } else {
            ss << ", ";
        }
        ss << c.name << " " << c.type;
    }
    ss << ");";
    return ss.str();
}

std::string InsertQuery(const models::TableSchema& meta, const std::list<std::string>& row_values) {
    std::stringstream ss;
    ss << "INSERT INTO " << meta.name << " (";
    bool is_first = true;
    for (const auto& c : meta.columns) {
        if (is_first) {
            is_first = false;
        } else {
            ss << ",";
        }
        ss << c.name;
    }
    ss << ") VALUES (";
    is_first = true;
    for (const auto& v : row_values) {
        if (is_first) {
            is_first = false;
        } else {
            ss << ",";
        }
        ss << v;
    }
    ss << ");";
    return ss.str();
}

} // namespace join_server::db::queries
