#include "sink_holder.hpp"

#include <iostream>
#include <fstream>
#include <thread>

#include <commands.hpp>
#include <pubsub/queue.hpp>
#include <utils/file.hpp>


namespace async::sinks {

namespace {

void stream_worker(std::shared_ptr<pubsub::Queue<std::string>> sink_ptr,
                   std::ostream& stream) {
    for (;;) {
        auto command = sink_ptr->PopNext();
        stream << command << std::endl;
    }
}

auto Init() {
    using sink_t = pubsub::Queue<command_t>;

    // setup command sinks (where to flush commands)
    auto file_sink_ptr = std::make_shared<sink_t>();
    auto log_sink_ptr = std::make_shared<sink_t>();

    // setup workers to consume commands from the sinks
    // commands from file sink are distributed between two file workers
    // all log commands are handled by log worker
    std::ofstream file1(utils::GenerateFileName(), std::ios_base::out);
    std::ofstream file2(utils::GenerateFileName(), std::ios_base::out);
    std::thread file_worker_thread1(stream_worker, file_sink_ptr, std::ref(file1));
    std::thread file_worker_thread2(stream_worker, file_sink_ptr, std::ref(file2));
    std::thread log_worker_thread(stream_worker, log_sink_ptr, std::ref(std::cout));
    file_worker_thread1.detach();
    file_worker_thread2.detach();
    log_worker_thread.detach();

    std::vector<std::shared_ptr<sink_t>> sinks = {
        file_sink_ptr, log_sink_ptr
    };
    return sinks;
}

} // namespace

SinksHolder& SinksHolder::GetInstance() {
    static SinksHolder instance;
    return instance;
}

SinksHolder::SinksHolder() {
    sinks_ = Init();
}

void SinksHolder::Flush(const command_t& command) {
    for (auto& sink : sinks_) {
        sink->Push(command);
    }
}

void SinksHolder::Flush(const std::list<command_t>& commands) {
    for (auto& sink : sinks_) {
        sink->Push(commands);
    }
}


} // namespace async::sinks
