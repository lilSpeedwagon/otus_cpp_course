#include "stream.hpp"


namespace async::workers {

void stream_worker(std::shared_ptr<pubsub::Queue<std::string>> sink_ptr,
                   std::ostream& stream) {
    for (;;) {
        auto command = sink_ptr->PopNext();
        stream << command << std::endl;
    }
}

} // namespace async::workers
