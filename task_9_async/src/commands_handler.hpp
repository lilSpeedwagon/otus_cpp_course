#pragma once

#include <istream>
#include <ostream>
#include <memory>
#include <string>

#include <containers/bulk_container.hpp>


namespace async {

/// @brief Bulk commands interpeteur.
/// Operates with commands blocks of static and dynamic sized.
class CommandsHandler {
public:
    using command_type = std::string;

    CommandsHandler(
        std::istream& input_stream,
        std::ostream& output_stream,
        std::shared_ptr<containers::BulkContainer<command_type>> commands_contaier,
        std::shared_ptr<containers::BulkContainer<command_type>> block_contaier);
    ~CommandsHandler();

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
    std::ostream& output_stream_;
    std::shared_ptr<containers::BulkContainer<command_type>> commands_contaier_;
    std::shared_ptr<containers::BulkContainer<command_type>> block_contaier_;
};

} // namespace async
