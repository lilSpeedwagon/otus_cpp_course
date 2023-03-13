#pragma once

#include <stdexcept>

#include "sqlite3.h"

#include <logging/logger.hpp>
#include <utils/env.hpp>



namespace join_server::db {

sqlite3* DbConnect() {
    static const char* kDbName = "::memory:";
    sqlite3* handle = nullptr;
    if (sqlite3_open(kDbName, &handle))
    {
        sqlite3_close(handle);
        throw std::runtime_error("Cannot connect to SQLITE3 in-memory DB instance");
    }
    return handle;
}

} // namespace join_server::db
