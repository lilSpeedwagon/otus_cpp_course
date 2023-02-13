#pragma once

#include <istream>
#include <ostream>
#include <memory>
#include <string>

#include <containers/bulk_container.hpp>
#include <pubsub/queue.hpp>


namespace async {

/// @brief Bulk commands interpeteur.
/// Operates with commands blocks of static and dynamic sized.
class CommandsHandler {
public:
    using command_type = std::string;
    using sink_ptr_type = std::shared_ptr<pubsub::Queue<command_type>>;

    CommandsHandler(
        std::istream& input_stream,
        std::shared_ptr<containers::BulkContainer<command_type>> commands_contaier,
        std::shared_ptr<containers::BulkContainer<command_type>> block_contaier);
    ~CommandsHandler();

    /// @brief Adds a new sink to flush commands into.
    void AddSink(const std::string& sink_name, sink_ptr_type sink_ptr);

    /// @brief Removes a previously added sink.
    void RemoveSink(const std::string& sink_name);

    /// @brief Runs commands I/O loop for bulk handling.
    void Run();

private:
    static const std::string kBlockBegin;
    static const std::string kBlockEnd;

    bool IsDynamicBlock() const;

    std::shared_ptr<containers::BulkContainer<command_type>> GetActiveContainer() const;

    void FlushCommands();

    void WriteCommands(const std::vector<command_type>& commands) const;

    size_t dynamic_blocks_opened_;
    std::istream& input_stream_;
    std::shared_ptr<containers::BulkContainer<command_type>> commands_contaier_;
    std::shared_ptr<containers::BulkContainer<command_type>> block_contaier_;
    std::unordered_map<std::string, sink_ptr_type> sink_map_;
};

} // namespace async
