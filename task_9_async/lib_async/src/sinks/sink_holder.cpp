#include "sink_holder.hpp"

#include <iostream>
#include <fstream>
#include <thread>

#include <commands.hpp>
#include <pubsub/queue.hpp>
#include <utils/file.hpp>


namespace async::sinks {

namespace {

void StreamWorker(std::shared_ptr<pubsub::Queue<std::string>> sink_ptr,
                  std::ostream& stream) {
    for (;;) {
        auto command = sink_ptr->PopNext();
        stream << command << std::endl;
    }
}

void FileStreamWorker(std::shared_ptr<pubsub::Queue<std::string>> sink_ptr) {
    std::ofstream file(utils::GenerateFileName(), std::ios_base::out);
    StreamWorker(sink_ptr, file);
}

void LogStreamWorker(std::shared_ptr<pubsub::Queue<std::string>> sink_ptr) {
    StreamWorker(sink_ptr, std::cout);
}

auto Init() {
    using sink_t = pubsub::Queue<command_t>;

    // setup command sinks (where to flush commands)
    auto file_sink_ptr = std::make_shared<sink_t>();
    auto log_sink_ptr = std::make_shared<sink_t>();

    // Setup workers to consume commands from the sinks.
    // Commands from file sink are distributed between two file workers
    // Commands from log sink are handled by a single log worker.
    std::thread file_worker_thread1(FileStreamWorker, file_sink_ptr);
    std::thread file_worker_thread2(FileStreamWorker, file_sink_ptr);
    std::thread log_worker_thread(LogStreamWorker, log_sink_ptr);
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
