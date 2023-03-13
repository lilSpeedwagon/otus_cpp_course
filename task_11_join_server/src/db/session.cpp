#include "session.hpp"

#include <stdexcept>

#include <db/errors.hpp>
#include <logging/logger.hpp>


namespace join_server::db {

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

} // namespace join_server::db
