#include "context_holder.hpp"

#include <optional>
#include <string>
#include <unordered_map>
#include <queue>

#include <handle.hpp>
#include <commands.hpp>


namespace async::context {

ContextHolder& ContextHolder::GetInstance() {
    static ContextHolder instance;
    return instance;
}

handle_t ContextHolder::CreateContext() {
    const auto handle = reinterpret_cast<handle_t>(context_counter_++);
    context_map_[handle] = {};
    return handle;
}

std::optional<Context> ContextHolder::GetContext(handle_t context) {
    if (auto it = context_map_.find(context);
        it != context_map_.end()) {
        return std::make_optional(it->second);
    }
    return std::nullopt;
}

bool ContextHolder::RemoveContext(handle_t context) {
    if (auto it = context_map_.find(context);
        it != context_map_.end()) {
        context_map_.erase(it);
        return true;
    }
    return false;
}

} // namespace async::context
