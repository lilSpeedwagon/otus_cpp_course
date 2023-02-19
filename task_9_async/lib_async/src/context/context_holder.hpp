#pragma once

#include <list>
#include <memory>
#include <optional>
#include <unordered_map>

#include <handle.hpp>
#include <commands.hpp>
#include <containers/bulk_container.hpp>


namespace async::context {

class Context {
public:
    Context(size_t block_size);
    ~Context() = default;
    void AddCommands(const char* commands_buffer, size_t size);
    
private:
    using container_ptr_t = std::shared_ptr<containers::BulkContainer<command_t>>;

    bool IsDynamicBlock() const;
    container_ptr_t GetActiveContainer() const;
    void AddCommand(command_t&& command);
    void FlushCommands();

    size_t dynamic_blocks_opened_;
    container_ptr_t dynamic_container_;
    container_ptr_t static_container_;
};

class ContextHolder {
public:
    static ContextHolder& GetInstance();

    handle_t CreateContext(size_t block_size);

    std::weak_ptr<Context> GetContext(handle_t context);

    bool RemoveContext(handle_t context);

private:
    ContextHolder() = default;

    size_t context_counter_;
    std::unordered_map<handle_t, std::shared_ptr<Context>> context_map_;
};

} // namespace async::context
