#include "handle_commands.hpp"

#include <containers/dynamic_container.hpp>
#include <containers/static_container.hpp>


namespace bulk_server::handlers {

void HandleCommands(const std::string& data) {
    auto begin_it = data.begin();
    for (auto it = data.begin(); it != data.end(); it++) {
        if (*it == '\n') {
            if (begin_it != it) {
                std::string command(begin_it, it);
                // add command
                begin_it = ++it;
            } else {
                begin_it++;
            }
        }
    }
    if (begin_it != data.end()) {
        std::string command(begin_it, data.end());
        // add command
    }
    // flush

    // TODO use containers logic here and flush when block is ready
}

} // namespace bulk_server::handlers
