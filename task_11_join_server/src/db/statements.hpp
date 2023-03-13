#pragma once

#include "sqlite3.h"

#include <db/errors.hpp>
#include <db/models/models.hpp>
#include <db/queries.hpp>
#include <db/utils.hpp>
#include <logging/logger.hpp>


namespace join_server::db {

void ExecuteStatement(sqlite3* handle, const std::string& statement) {
    LOG_DEBUG() << "Executing query: " << statement;
    Sql3Error error;
    auto res = sqlite3_exec(handle, statement.c_str(), nullptr, nullptr, error.GetBufferPtr());
    if (res != SQLITE_OK) {
        throw std::runtime_error(error.GetMsg());
    }
}

} // namespace join_server::db
