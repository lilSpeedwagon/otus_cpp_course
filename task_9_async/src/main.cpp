#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <thread>
#include <vector>

#include <commands_handler.hpp>
#include <containers/dynamic_bulk_container.hpp>
#include <containers/static_bulk_container.hpp>
#include <pubsub/queue.hpp>
#include <utils/time.hpp>
#include <workers/stream.hpp>


namespace {

using command_type = async::CommandsHandler::command_type;

/// @brief Generates a randomized file name in format 
/// .\YYYY-MM-DDThh:mm:ss_XXXXXXXXXXXX" where X is a random digit
std::string GenerateFileName() {
    static std::random_device rd;
    static const size_t kRandomStringSize = 12;

    std::stringstream ss;
    ss << "./" << async::utils::GetTimeString() << '_';
    std::uniform_int_distribution<char> dist(0, 9);
    for (size_t i = 0; i < kRandomStringSize; i++) {
        ss << std::to_string(dist(rd));
    }
    return ss.str();
}

} // namespace

int main() {
    std::cout << "Enter bulk block size: ";
    size_t block_size;
    std::cin >> block_size;
    if (block_size < 1) {
        throw std::runtime_error("invalid block size");
    }
    
    // setup command containers
    auto static_container_ptr = 
        std::make_shared<async::containers::StaticBulkContainer<command_type>>(block_size);
    auto dynamic_container_ptr = 
        std::make_shared<async::containers::DynamicBulkContainer<command_type>>();

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

    // setup and run commands handler main loop
    async::CommandsHandler handler(std::cin, static_container_ptr, dynamic_container_ptr);
    handler.AddSink("file", file_sink_ptr);
    handler.AddSink("log", log_sink_ptr);
    handler.Run();

    // wait some to let workers end their's work
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return 0;
}
