#pragma once

#include <memory>

#include <commands.hpp>
#include <pubsub/queue.hpp>


namespace bulk_server::workers {

using sink_t = pubsub::Queue<command_block_t>;

void FileStreamWorker(std::shared_ptr<sink_t> sink_ptr);

void LogStreamWorker(std::shared_ptr<sink_t> sink_ptr);

} // namespace bulk_server::workers
