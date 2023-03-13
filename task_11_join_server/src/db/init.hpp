#pragma once

#include "sqlite3.h"

#include <db/models/models.hpp>
#include <db/statements.hpp>
#include <db/queries.hpp>
#include <db/utils.hpp>
#include <logging/logger.hpp>


namespace join_server::db {

void Init() {
    LOG_DEBUG() << "Prepare db schema";

    auto handle = DbConnect();

    auto create_a_query = queries::CreateTableQuery(models::kAMetadata);
    auto create_b_query = queries::CreateTableQuery(models::kBMetadata);

    ExecuteStatement(handle, create_a_query);
    ExecuteStatement(handle, create_b_query);
}

} // namespace join_server::db
