#pragma once

#include <optional>
#include <unordered_map>
#include <queue>

#include <handle.hpp>
#include <commands.hpp>


namespace async::context {

struct Context {
    size_t block_size;
    std::queue<command_t> commands;
};

class ContextHolder {
public:
    static ContextHolder& GetInstance();

    handle_t CreateContext();

    std::optional<Context> GetContext(handle_t context);

    bool RemoveContext(handle_t context);

private:
    ContextHolder() = default;

    size_t context_counter_;
    std::unordered_map<handle_t, Context> context_map_;
};

} // namespace async::context
