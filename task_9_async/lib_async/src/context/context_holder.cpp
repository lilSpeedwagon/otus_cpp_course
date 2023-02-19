#include "context_holder.hpp"

#include <optional>
#include <string>
#include <unordered_map>
#include <queue>

#include <handle.hpp>
#include <commands.hpp>
#include <containers/dynamic_bulk_container.hpp>
#include <containers/static_bulk_container.hpp>
#include <sinks/sink_holder.hpp>


namespace async::context {

Context::Context(size_t block_size) : dynamic_blocks_opened_(0) {
    dynamic_container_ = std::make_shared<containers::StaticBulkContainer<command_t>>(block_size);
    static_container_ = std::make_shared<containers::DynamicBulkContainer<command_t>>();
}

void Context::AddCommands(const char* commands_buffer, size_t size) {
    size_t begin_cursor = 0;
    for (size_t cursor = 0; cursor < size; cursor++) {
        if (*(commands_buffer + cursor) == '\n') {
            if (begin_cursor != cursor) {
                std::string command(commands_buffer + begin_cursor, cursor - begin_cursor);
                AddCommand(std::move(command));
                begin_cursor = ++cursor;
            } else {
                begin_cursor++;
            }
        }
    }
    if (begin_cursor < size) {
        std::string command(commands_buffer + begin_cursor, size - begin_cursor);
        AddCommand(std::move(command));
    }
    FlushCommands();
}

void Context::AddCommand(command_t&& command) {
    static const std::string kBlockBegin = "{";
    static const std::string kBlockEnd = "}";
    
    if (command == kBlockBegin) {
        if (!IsDynamicBlock()) {
            FlushCommands();
        }
        dynamic_blocks_opened_++;
    } else if (command == kBlockEnd) {
        if (!IsDynamicBlock()) {
            throw std::runtime_error("unexpected command");
        }
        if (dynamic_blocks_opened_ == 1) {
            FlushCommands();
        }
        dynamic_blocks_opened_--;
    } else {
        auto container_ptr = GetActiveContainer();
        bool is_full = !container_ptr->Add(command);
        if (is_full) {
            FlushCommands();
        }
    }
}

bool Context::IsDynamicBlock() const {
    return dynamic_blocks_opened_ > 0;
}

Context::container_ptr_t Context::GetActiveContainer() const {
    return IsDynamicBlock() ? dynamic_container_ : static_container_;
}

void Context::FlushCommands() {
    auto container_ptr = GetActiveContainer();
    auto sinks = sinks::SinksHolder::GetInstance();
    sinks.Flush(container_ptr->GetItems());
    container_ptr->Clear();
}

ContextHolder& ContextHolder::GetInstance() {
    static ContextHolder instance;
    return instance;
}

handle_t ContextHolder::CreateContext(size_t block_size) {
    const auto handle = reinterpret_cast<handle_t>(context_counter_++);
    context_map_[handle] = std::make_shared<Context>(block_size);
    return handle;
}

std::weak_ptr<Context> ContextHolder::GetContext(handle_t context) {
    if (auto it = context_map_.find(context);
        it != context_map_.end()) {
        return std::weak_ptr(it->second);
    }
    return std::weak_ptr<Context>();
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
