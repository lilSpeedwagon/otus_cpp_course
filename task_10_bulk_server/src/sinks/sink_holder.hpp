#pragma once

#include <list>
#include <memory>
#include <vector>

#include <commands.hpp>
#include <pubsub/queue.hpp>


namespace bulk_server::sinks {

class SinksHolder {
public:
    static SinksHolder& GetInstance();

    void Flush(const command_block_t& commands);

private:
    using sink_t = pubsub::Queue<command_block_t>;

    SinksHolder();

    std::vector<std::shared_ptr<sink_t>> sinks_;
};

} // namespace bulk_server::sinks
