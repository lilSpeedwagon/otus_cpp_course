#include "workers.hpp"

#include <iostream>
#include <fstream>

#include <utils/file.hpp>


namespace bulk_server::workers {

namespace {

void WriteBlockToStream(const command_block_t& commands, std::ostream& stream) {
    bool is_first = true;
    for (const auto& c : commands) {
        if (is_first) {
            is_first = false;
        } else {
            stream << ", ";
        }
        stream << c;
    }
    stream << std::endl;
}

} // namespace

void FileStreamWorker(std::shared_ptr<sink_t> sink_ptr) {
    for (;;) {
        auto command_block = sink_ptr->PopNext();
        std::ofstream file(utils::GenerateFileName(), std::ios_base::out);
        WriteBlockToStream(command_block, file);
    }
}

void LogStreamWorker(std::shared_ptr<sink_t> sink_ptr) {
    for (;;) {
        auto command_block = sink_ptr->PopNext();
        WriteBlockToStream(command_block, std::cout);
    }
}

} // namespace bulk_server::workers
