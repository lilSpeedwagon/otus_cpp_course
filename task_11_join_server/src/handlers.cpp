#include "handlers.hpp"

#include <stdexcept>
#include <sstream>

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

auto RowsToString(const std::vector<db::Row>& rows) {
    std::stringstream ss;
    for (const auto& row : rows) {
        bool is_first = true;
        for (const auto& column : row) {
            if (is_first) {
                is_first = false;
            } else {
                ss << ',';
            }
            ss << column.value.value_or("");
        }
        ss << '\n';
    }
    return ss.str();
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

std::string HandleIntersetion() {
    const auto& lhs_meta = db::models::kAMetadata;
    const auto& rhs_meta = db::models::kBMetadata;
    const auto& on_lhs = lhs_meta.columns[0].name;
    const auto& on_rhs = rhs_meta.columns[0].name;
    const std::list<std::string> lhs_fields = {lhs_meta.columns[1].name};
    const std::list<std::string> rhs_fields = {rhs_meta.columns[1].name};

    const auto query = db::queries::JoinQuery(
        lhs_meta, rhs_meta, on_lhs, on_rhs, lhs_fields, rhs_fields);

    db::Session session;
    auto rows = session.ExecuteStatementWithResult(query);
    return RowsToString(rows);
}

std::string HandleDifference() {
    const auto& lhs_meta = db::models::kAMetadata;
    const auto& rhs_meta = db::models::kBMetadata;
    const auto& on_lhs = lhs_meta.columns[0].name;
    const auto& on_rhs = rhs_meta.columns[0].name;
    const std::list<std::string> lhs_fields = {lhs_meta.columns[1].name};
    const std::list<std::string> rhs_fields = {rhs_meta.columns[1].name};

    const auto query = db::queries::JoinExcludeQuery(
        lhs_meta, rhs_meta, on_lhs, on_rhs, lhs_fields, rhs_fields);

    db::Session session;
    auto rows = session.ExecuteStatementWithResult(query);
    return RowsToString(rows);
}

} // namespace join_server::handlers
