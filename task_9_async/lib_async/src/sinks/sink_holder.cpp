#pragma once

#include <queue>
#include <vector>

#include <commands.hpp>
#include <pubsub/queue.hpp>


void stream_worker(std::shared_ptr<pubsub::Queue<std::string>> sink_ptr,
                   std::ostream& stream) {
    for (;;) {
        auto command = sink_ptr->PopNext();
        stream << command << std::endl;
    }
}

namespace async::sinks {

SinksHolder& SinksHolder::GetInstance() {
    static SinksHolder instance;
    return instance;
}

void SinksHolder::Flush(const command_t& command) {

}

void SinksHolder::Flush(const std::queue<command_t>& command) {

}


private:
    SinksHolder() {}

    void Init() {
        // setup command sinks (where to flush commands)
        auto file_sink_ptr = std::make_shared<async::pubsub::Queue<command_type>>();
        auto log_sink_ptr = std::make_shared<async::pubsub::Queue<command_type>>();

        // setup workers to consume commands from the sinks
        // commands from file sink are distributed between two file workers
        // all log commands are handled by log worker
        std::ofstream file1(GenerateFileName(), std::ios_base::out);
        std::ofstream file2(GenerateFileName(), std::ios_base::out);
        std::thread file_worker_thread1(async::workers::stream_worker, file_sink_ptr, std::ref(file1));
        std::thread file_worker_thread2(async::workers::stream_worker, file_sink_ptr, std::ref(file2));
        std::thread log_worker_thread(async::workers::stream_worker, log_sink_ptr, std::ref(std::cout));
        file_worker_thread1.detach();
        file_worker_thread2.detach();
        log_worker_thread.detach();
    }
};

} // namespace async::sinks
