#include "init.hpp"

#include <db/models/models.hpp>
#include <db/queries.hpp>
#include <db/session.hpp>
#include <logging/logger.hpp>


namespace join_server::db {

void Init() {
    LOG_DEBUG() << "Prepare db schema";

    Session session;

    auto create_a_query = queries::CreateTableQuery(models::kAMetadata);
    auto create_b_query = queries::CreateTableQuery(models::kBMetadata);

    session.ExecuteStatement(create_a_query);
    session.ExecuteStatement(create_b_query);
}

} // namespace join_server::db
