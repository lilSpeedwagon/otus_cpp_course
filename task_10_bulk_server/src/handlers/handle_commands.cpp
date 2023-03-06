#include "handle_commands.hpp"

#include <memory>

#include <commands.hpp>
#include <containers/dynamic_container.hpp>
#include <containers/static_container.hpp>
#include <logging/logger.hpp>
#include <sinks/sink_holder.hpp>


namespace bulk_server::handlers {

namespace {

static const size_t kDefaultBlockSize = 3;

} // namespace

CommandsHandler& CommandsHandler::GetInstance() {
    static CommandsHandler instance;
    return instance;
}

CommandsHandler::CommandsHandler() : dynamic_blocks_opened_(false) {
    dynamic_container_ = std::make_shared<containers::DynamicBulkContainer<command_t>>();
    static_container_ = std::make_shared<containers::StaticBulkContainer<command_t>>(kDefaultBlockSize);
}

void CommandsHandler::SetBlockSize(size_t block_size) {
    if (static_container_->Size() > 0) {
        auto sink = sinks::SinksHolder::GetInstance();
        sink.Flush(static_container_->GetItems());
    }
    static_container_ = std::make_shared<containers::StaticBulkContainer<command_t>>(block_size);
}

void CommandsHandler::AddCommands(const std::string& data) {
    auto begin_it = data.begin();
    for (auto it = data.begin(); it != data.end(); it++) {
        if (*it == '\n') {
            if (begin_it != it) {
                std::string command(begin_it, it);
                AddCommand(std::move(command));
                begin_it = it + 1;
            } else {
                begin_it++;
            }
        }
    }
    if (begin_it != data.end()) {
        std::string command(begin_it, data.end());
        AddCommand(std::move(command));
    }
    FlushCommands();
}

void CommandsHandler::AddCommand(command_t&& command) {
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

bool CommandsHandler::IsDynamicBlock() const {
    return dynamic_blocks_opened_ > 0;
}

CommandsHandler::container_ptr_t CommandsHandler::GetActiveContainer() const {
    return IsDynamicBlock() ? dynamic_container_ : static_container_;
}

void CommandsHandler::FlushCommands() {
    auto container_ptr = GetActiveContainer();
    auto items = container_ptr->GetItems();
    LOG_INFO() << "Flushing " << items.size() << " commands";
    auto sinks = sinks::SinksHolder::GetInstance();
    sinks.Flush(items);
    container_ptr->Clear();
}

} // namespace bulk_server::handlers
