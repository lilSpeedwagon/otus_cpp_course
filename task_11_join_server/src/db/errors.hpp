#pragma once

#include "sqlite3.h"


namespace join_server::db {

/// @brief sqlite3 error message wrapper for proper memory cleanup.
/// Use 'GetBufferPtr()' to obtain a buffer pointer and pass it to 
/// sqlite3 functions such as 'sqlite3_exec()'.
class Sql3Error {
public:
    Sql3Error() = default;
    ~Sql3Error();

    /// @brief Get error message.
    /// @return error message string
    const char* GetMsg() const;

    /// @brief Get error message buffer pointer.
    /// @return pointer to the internal buffer
    char** GetBufferPtr();

private:
    char* buffer_;
};

} // namespace join_server::db
