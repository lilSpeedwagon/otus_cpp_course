#include "commands_handler.hpp"

#include <stdexcept>


namespace bulk {

const std::string CommandsHandler::kBlockBegin = "{";
const std::string CommandsHandler::kBlockEnd = "}";

CommandsHandler::CommandsHandler(
    std::istream& input_stream,
    std::ostream& output_stream,
    std::shared_ptr<BulkContainer<command_type>> commands_contaier,
    std::shared_ptr<BulkContainer<command_type>> block_contaier)
    : dynamic_blocks_opened_(0), 
        input_stream_(input_stream), output_stream_(output_stream),
        commands_contaier_(commands_contaier), block_contaier_(block_contaier) {}

CommandsHandler::~CommandsHandler() {}

void CommandsHandler::Run() {
    std::string command;
    for (;;) {
        input_stream_ >> command;
        if (input_stream_.eof()) {
            FlushCommands();
            return;
        }

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
}
bool CommandsHandler::IsDynamicBlock() const {
    return dynamic_blocks_opened_ > 0;
}

std::shared_ptr<BulkContainer<CommandsHandler::command_type>>
    CommandsHandler::GetActiveContainer() const {
    return IsDynamicBlock() ? block_contaier_ : commands_contaier_;
}

void CommandsHandler::FlushCommands() {
    auto container_ptr = GetActiveContainer();
    WriteCommands(container_ptr->GetItems());
    container_ptr->Clear();
}

void CommandsHandler::WriteCommands(const std::vector<command_type>& commands) const {
    bool is_first = true;
    for (const auto& c : commands) {
        if (is_first) {
            is_first = false;
        } else {
            output_stream_ << ',';
        }
        output_stream_ << c;
    }
    output_stream_ << '\n';
}

} // namespace bulk
