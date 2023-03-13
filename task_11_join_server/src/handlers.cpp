#include "handlers.hpp"

#include <stdexcept>

#include <db/models/models.hpp>
#include <db/queries.hpp>
#include <db/session.hpp>


namespace join_server::handlers {

namespace {

auto GetMetadata(const std::string& table_name) {
    if (table_name == db::models::kAMetadata.name) {
        return db::models::kAMetadata;
    } else if (table_name == db::models::kBMetadata.name) {
        return db::models::kBMetadata;
    }
    throw std::runtime_error(std::string("Unknown table: ") + table_name);
}

} // namespace

std::string HandleInsert(const std::string& table_name, const std::string& id, const std::string& value) {
    const auto meta = GetMetadata(table_name);
    const auto query = db::queries::InsertQuery(meta, {id, value});

    db::Session session;
    session.ExecuteStatement(query);

    return "OK";
}

std::string HandleTruncate(const std::string& table_name) {
    const auto meta = GetMetadata(table_name);
    const auto query = db::queries::TruncateQuery(meta);

    db::Session session;
    session.ExecuteStatement(query);

    return "OK";
}

} // namespace join_server::handlers
