#pragma once

#include <string>

#include <db/queries.hpp>
#include <db/statements.hpp>


namespace join_server::handlers {

std::string HandleInsert(const std::string& table_name, const std::string& id, const std::string& value) {
    std::string query;
    if (table_name == db::models::kAMetadata.name) {
        query = db::queries::InsertQuery(db::models::kAMetadata, {id, value});
    } else if (table_name == db::models::kBMetadata.name) {
        query = db::queries::InsertQuery(db::models::kBMetadata, {id, value});
    } else {
        throw std::runtime_error(std::string("Unknown table: ") + table_name);
    }

    auto handle = db::DbConnect();
    db::ExecuteStatement(handle, query);

    return "OK";
}

} // namespace join_server::handlers
