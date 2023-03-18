#include "sink_holder.hpp"

#include <thread>

#include <logging/logger.hpp>
#include <workers/workers.hpp>


namespace bulk_server::sinks {

namespace {

auto Init() {
    using sink_t = pubsub::Queue<command_block_t>;

    LOG_INFO() << "Init sinks...";

    // setup command sinks (where to flush commands)
    auto file_sink_ptr = std::make_shared<sink_t>();
    auto log_sink_ptr = std::make_shared<sink_t>();

    // Setup workers to consume commands from the sinks.
    // Commands from file sink are distributed between two file workers
    // Commands from log sink are handled by a single log worker.
    std::thread file_worker_thread1(workers::FileStreamWorker, file_sink_ptr);
    std::thread file_worker_thread2(workers::FileStreamWorker, file_sink_ptr);
    std::thread log_worker_thread(workers::LogStreamWorker, log_sink_ptr);
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

void SinksHolder::Flush(const command_block_t& commands) {
    for (auto& sink : sinks_) {
        sink->Push(commands);
    }
}

} // namespace bulk_server::sinks
