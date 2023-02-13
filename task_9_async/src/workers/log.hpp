#pragma once

#include <memory>
#include <string>

#include <pubsub/queue.hpp>


namespace async::workers {

void log_worker(std::shared_ptr<pubsub::Queue<std::string>> sink_ptr);

} // namespace async::workers
