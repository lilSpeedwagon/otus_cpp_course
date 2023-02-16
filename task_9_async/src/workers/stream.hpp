#pragma once

#include <iostream>
#include <memory>

#include <pubsub/queue.hpp>


namespace async::workers {

/// @brief thread worker function awaiting for commands coming to sink.
/// Every new command is poped from the sink and put to the stream.
/// @param sink_ptr sink pointer
/// @param stream abstract output stream
void stream_worker(std::shared_ptr<pubsub::Queue<std::string>> sink_ptr,
                   std::ostream& stream);

} // namespace async::workers
