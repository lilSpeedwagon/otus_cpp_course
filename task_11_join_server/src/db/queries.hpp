#pragma once

#include <string>

#include <db/models/base.hpp>


namespace join_server::db::queries {

/// @brief Create SQL query 'CREATE TABLE IF NOT EXISTS <meta.name> (<meta.columns>);'
/// @param meta new table metadata
/// @return query string
std::string CreateTableQuery(const models::TableSchema& meta);

/// @brief Create SQL query 'INSERT INTO <meta.name> (<meta.columns>) VALUES (<row_values>);'
/// @param meta table metadata
/// @param row_values list of row values to insert
/// @return query string
std::string InsertQuery(const models::TableSchema& meta, const std::list<std::string>& row_values);

/// @brief Create SQL query 'DELETE FROM <meta.name>;'
/// @param meta table metadata
/// @return query string
std::string TruncateQuery(const db::models::TableSchema& meta);

/// @brief Create SQL join for 2 tables.
/// Query: 'SELECT <on_lhs>, <lhs_fields>, <rhs_fields> FROM <meta_lhs.name>
/// JOIN <meta_rhs.name> ON <on_lhs> = <on_rhs>;'
/// @param meta_lhs left joinable table metadata
/// @param meta_rhs right joinable table metadata
/// @param on_lhs column name of the left table to join on
/// @param on_rhs column name of the right table to join on
/// @param lhs_fields left table fields to select
/// @param rhs_fields right table fields to select
/// @return query string
std::string JoinQuery(const db::models::TableSchema& meta_lhs,
                      const db::models::TableSchema& meta_rhs,
                      const std::string& on_lhs,
                      const std::string& on_rhs,
                      const std::list<std::string>& lhs_fields,
                      const std::list<std::string>& rhs_fields);

/// @brief Create SQL query for symmetric difference between 2 tables.
/// Query: 'SELECT <on_lhs>, <lhs_fields>, "" FROM <meta_lhs.name>
/// WHERE <on_lhs> NOT IN (SELECT <on_rhs> FROM <meta_rhs.name>) UNION
/// SELECT <on_rhs>, "", <rhs_fields>, FROM <meta_rhs.name>
/// WHERE <on_rhs> NOT IN (SELECT <on_lhs> FROM <meta_rlhs.name>);'
/// @param meta_lhs left joinable table metadata
/// @param meta_rhs right joinable table metadata
/// @param on_lhs column name of the left table to join on
/// @param on_rhs column name of the right table to join on
/// @param lhs_fields left table fields to select
/// @param rhs_fields right table fields to select
/// @return query string
std::string JoinExcludeQuery(const db::models::TableSchema& meta_lhs,
                             const db::models::TableSchema& meta_rhs,
                             const std::string& on_lhs,
                             const std::string& on_rhs,
                             const std::list<std::string>& lhs_fields,
                             const std::list<std::string>& rhs_fields);

} // namespace join_server::db::queries
