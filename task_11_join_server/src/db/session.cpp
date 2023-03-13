#include "session.hpp"

#include <optional>
#include <stdexcept>

#include <db/errors.hpp>
#include <db/results.hpp>
#include <logging/logger.hpp>


namespace join_server::db {

namespace {

int Callback(void *data, int argc, char **argv, char **columns_names) {
    auto rows_ptr = reinterpret_cast<std::vector<Row>*>(data);
    Row row;
    for (int i = 0; i < argc; i++) {
        auto value = argv[i];
        auto column_name = columns_names[i];
        Column column {
            std::string(column_name),
            value ? std::make_optional<std::string>(value) : std::nullopt,
        };
        row.emplace_back(std::move(column));
    }
    rows_ptr->emplace_back(std::move(row));
    return 0;
}

} // namespace

Session::Session() : handle_(nullptr) {
    static const char* kDbName = "::memory:";

    if (sqlite3_open(kDbName, &handle_))
    {
        sqlite3_close(handle_);
        throw std::runtime_error("Cannot connect to SQLITE3 in-memory DB instance");
    }
}

Session::~Session() {
    sqlite3_close(handle_);
}

void Session::ExecuteStatement(const std::string& statement) {
    LOG_DEBUG() << "Executing query: " << statement;
    Sql3Error error;
    auto res = sqlite3_exec(handle_, statement.c_str(), nullptr, nullptr, error.GetBufferPtr());
    if (res != SQLITE_OK) {
        throw std::runtime_error(error.GetMsg());
    }
}

std::vector<Row> Session::ExecuteStatementWithResult(const std::string& statement) {
    LOG_DEBUG() << "Executing query: " << statement;
    Sql3Error error;
    std::vector<Row> rows;
    auto res = sqlite3_exec(handle_, statement.c_str(), Callback,
                            reinterpret_cast<void*>(&rows), error.GetBufferPtr());
    if (res != SQLITE_OK) {
        throw std::runtime_error(error.GetMsg());
    }
    return rows;
}

} // namespace join_server::db
