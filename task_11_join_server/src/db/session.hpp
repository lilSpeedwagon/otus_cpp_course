#pragma once

#include <string>

#include <boost/noncopyable.hpp>
#include "sqlite3.h"


namespace join_server::db {

class Session : private boost::noncopyable {
public:
    Session();
    ~Session();

    void ExecuteStatement(const std::string& statement);

private:
    sqlite3* handle_;
};

} // namespace join_server::db
