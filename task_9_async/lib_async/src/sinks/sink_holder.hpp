#pragma once

#include <list>
#include <memory>
#include <vector>

#include <commands.hpp>
#include <pubsub/queue.hpp>


namespace async::sinks {

class SinksHolder {
public:
    static SinksHolder& GetInstance();

    void Flush(const command_t& command);
    void Flush(const std::list<command_t>& command);

private:
    using sink_t = async::pubsub::Queue<command_t>;

    SinksHolder();

    std::vector<std::shared_ptr<sink_t>> sinks_;
};

} // namespace async::sinks