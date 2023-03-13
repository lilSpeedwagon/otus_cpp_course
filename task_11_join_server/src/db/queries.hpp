#pragma once

#include <string>

#include <db/models/base.hpp>


namespace join_server::db::queries {

std::string CreateTableQuery(const models::TableSchema& meta);

std::string InsertQuery(const models::TableSchema& meta, const std::list<std::string>& row_values);

std::string TruncateQuery(const db::models::TableSchema& meta);

} // namespace join_server::db::queries
