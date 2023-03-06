#pragma once

#include <memory>
#include <string>

#include <boost/noncopyable.hpp>

#include <containers/bulk_container.hpp>
#include <containers/dynamic_container.hpp>
#include <containers/static_container.hpp>
#include <commands.hpp>


namespace bulk_server::handlers {

class CommandsHandler : public boost::noncopyable {
public:
    using container_ptr_t = std::shared_ptr<containers::BulkContainer<command_t>>;

    static CommandsHandler& GetInstance();

    void SetBlockSize(size_t block_size);
    void AddCommands(const std::string& data);

private:
    CommandsHandler();
    void AddCommand(command_t&& command);
    bool IsDynamicBlock() const;
    container_ptr_t GetActiveContainer() const;
    void FlushCommands();

    size_t dynamic_blocks_opened_;
    container_ptr_t dynamic_container_;
    container_ptr_t static_container_;
};

} // namespace bulk_server::handlers
