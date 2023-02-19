#pragma once

#include <handle.hpp>


namespace async {

/// @brief Create new context for commands.
/// @param bulk size of a commands block
/// @return context handler
handle_t Connect(size_t bulk);

/// @brief Put new command to the context.
/// @param handle context handler
/// @param data command buffer
/// @param size size of command buffer
/// @return non-zero in case of success, zero if context is not found
size_t Receive(handle_t handle, const char *data, size_t size);

/// @brief Destroy context.
/// @param handle context handler
/// @return non-zero in case of success, zero if context is not found
size_t Disconnect(handle_t handle);

}  // namespace async
