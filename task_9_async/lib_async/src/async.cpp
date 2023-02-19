#include <async.hpp>

#include <context/context_holder.hpp>
#include <commands.hpp>


namespace async {

handle_t Connect(size_t bulk) {
    auto& context_holder = ContextQueueHolder::GetInstance();
    return context_holder.CreateContext();
}

size_t Receive(handle_t handle, const char *data, size_t size) {
    auto& context_holder = ContextQueueHolder::GetInstance();
    auto context_opt = context_holder.GetContext(handle);
    if (!context_opt.has_value()) {
        return 0;
    }

    auto& context = context_opt.value();
    command_t command(data, size);
    context.commands.push(std::move(command)); // emplace ?
    if (context.commands.size() >= context.block_size) {
        std::queue<command_t> queue;
        std::swap(queue, context.queue);
        std::async([]() {
            // send to topic
        });
    }
    return 1;
}

size_t Disconnect(handle_t handle) {
    auto& context_holder = ContextQueueHolder::GetInstance();
    return context_holder.RemoveContext(handle) ? 1 : 0;
}

} // namespace async
