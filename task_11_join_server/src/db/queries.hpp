#pragma once

#include <string>

#include <db/models/base.hpp>


namespace join_server::db::queries {

std::string CreateTableQuery(const models::TableSchema& meta);

std::string InsertQuery(const models::TableSchema& meta, const std::list<std::string>& row_values);

std::string TruncateQuery(const db::models::TableSchema& meta);

std::string JoinQuery(const db::models::TableSchema& meta_lhs,
                      const db::models::TableSchema& meta_rhs,
                      const std::string& on_lhs,
                      const std::string& on_rhs,
                      const std::list<std::string>& lhs_fields,
                      const std::list<std::string>& rhs_fields);

std::string JoinExcludeQuery(const db::models::TableSchema& meta_lhs,
                             const db::models::TableSchema& meta_rhs,
                             const std::string& on_lhs,
                             const std::string& on_rhs,
                             const std::list<std::string>& lhs_fields,
                             const std::list<std::string>& rhs_fields);

} // namespace join_server::db::queries
