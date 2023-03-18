#pragma once

#include <string>
#include <vector>

#include <boost/noncopyable.hpp>
#include "sqlite3.h"

#include <db/results.hpp>


namespace join_server::db {

/// @brief SQL database session.
class Session : private boost::noncopyable {
public:
    Session();
    ~Session();

    /// @brief Execute SQL query.
    /// @param statement query
    void ExecuteStatement(const std::string& statement);

    /// @brief Execute SQL query and return resulting rows.
    /// @param statement query
    /// @return resulting rows
    std::vector<Row> ExecuteStatementWithResult(const std::string& statement);

private:
    sqlite3* handle_;
};

} // namespace join_server::db
