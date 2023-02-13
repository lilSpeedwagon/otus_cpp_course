#include "log.hpp"

#include <iostream>


namespace async::workers {

void log_worker(std::shared_ptr<pubsub::Queue<std::string>> sink_ptr) {
    for (;;) {
        auto command = sink_ptr->PopNext();
        std::cout << command << '\n';
    }
}

} // namespace async::workers
