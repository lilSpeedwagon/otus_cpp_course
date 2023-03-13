#pragma once

#include "sqlite3.h"


namespace join_server::db {

class Sql3Error {
public:
    Sql3Error() {}
    ~Sql3Error() {
        if (buffer_ != nullptr) {
            sqlite3_free(buffer_);
        }
    }

    const char* GetMsg() const {
        return buffer_;
    }

    char** GetBufferPtr() {
        return &buffer_;
    }

private:
    char* buffer_;
};

} // namespace join_server::db
