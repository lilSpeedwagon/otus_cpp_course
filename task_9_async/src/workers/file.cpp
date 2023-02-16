#include "file.hpp"

#include <chrono>
#include <fstream>
#include <random>
#include <sstream>

#include <utils/time.hpp>


namespace async::workers {

namespace {

/// @brief Generates a randomized file in format .\YYYY-MM-DDThh:mm:ss_XXXXXXXXXXXX
/// where X is a random digit
std::string GenerateFileName() {
    static std::random_device rd;
    static const size_t kRandomStringSize = 12;

    std::stringstream ss;
    ss << ".\\" << utils::GetTimeString() << '_';
    std::uniform_int_distribution<char> dist(0, 9);
    for (size_t i = 0; i < kRandomStringSize; i++) {
        ss << dist(rd);
    }
    return ss.str();
}

} // namespace

void file_worker(std::shared_ptr<pubsub::Queue<std::string>> sink_ptr) {
    std::string file_name = GenerateFileName();
    std::ofstream file(file_name);
    for (;;) {
        auto command = sink_ptr->PopNext();
        file << command << '\n';        
    }
}

} // namespace async::workers
