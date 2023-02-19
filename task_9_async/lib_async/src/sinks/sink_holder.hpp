#pragma once

#include <memory>
#include <queue>
#include <vector>

#include <commands.hpp>
#include <pubsub/queue.hpp>


namespace async::sinks {

class SinksHolder {
public:
    static SinksHolder& GetInstance();

    void Flush(const command_t& command);
    void Flush(const std::queue<command_t>& command);

private:
    using sink_t = async::pubsub::Queue<command_type>;

    SinksHolder();

    std::vector<std::shared_ptr<simk_t>> sinks_;
};

} // namespace async::sinks