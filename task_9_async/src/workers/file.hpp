#pragma once 

#include <memory>

#include <pubsub/queue.hpp>


namespace async::workers {

void file_worker(std::shared_ptr<pubsub::Queue<std::string>> sink_ptr);

} // namespace async::workers
