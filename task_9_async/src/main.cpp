#include <iostream>
#include <memory>

#include <commands_handler.hpp>
#include <containers/dynamic_bulk_container.hpp>
#include <containers/static_bulk_container.hpp>


int main() {
    std::cout << "Enter bulk block size: ";
    size_t block_size;
    std::cin >> block_size;
    if (block_size < 1) {
        throw std::runtime_error("invalid block size");
    }

    using command_type = bulk::CommandsHandler::command_type;
    auto static_container_ptr = 
        std::make_shared<bulk::StaticBulkContainer<command_type>>(block_size);
    auto dynamic_container_ptr = 
        std::make_shared<bulk::DynamicBulkContainer<command_type>>();

    bulk::CommandsHandler handler(
        std::cin, std::cout, static_container_ptr, dynamic_container_ptr);
    handler.Run();

    return 0;
}
