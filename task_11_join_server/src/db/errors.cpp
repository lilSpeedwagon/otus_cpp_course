#include "errors.hpp"


namespace join_server::db {

Sql3Error::~Sql3Error() {
    if (buffer_ != nullptr) {
        sqlite3_free(buffer_);
    }
}

const char* Sql3Error::GetMsg() const {
    return buffer_;
}

char** Sql3Error::GetBufferPtr() {
    return &buffer_;
}

} // namespace join_server::db
