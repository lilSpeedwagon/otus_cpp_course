#include <async.hpp>

#include <future>

#include <commands.hpp>
#include <context/context_holder.hpp>
#include <sinks/sink_holder.hpp>


namespace async {

handle_t Connect(size_t block_size) {
    auto& context_holder = context::ContextHolder::GetInstance();
    return context_holder.CreateContext(block_size);
}

size_t Receive(handle_t handle, const char *data, size_t size) {
    auto& context_holder = context::ContextHolder::GetInstance();
    auto context_opt = context_holder.GetContext(handle);
    if (!context_opt.has_value()) {
        return 0;
    }

    auto& context = context_opt.value();
    command_t command(data, size);
    context.commands.emplace_back(std::move(command));
    if (context.commands.size() >= context.block_size) {
        auto sinks = sinks::SinksHolder::GetInstance();
        sinks.Flush(context.commands);
        context.commands.clear();
    }
    return 1;
}

size_t Disconnect(handle_t handle) {
    auto& context_holder = context::ContextHolder::GetInstance();
    return context_holder.RemoveContext(handle) ? 1 : 0;
}

} // namespace async
