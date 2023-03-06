#include <iostream>
#include <stdexcept>

#include <boost/asio.hpp>

#include <handlers/handle_commands.hpp>
#include <tcp/tcp_server.hpp>


void Handle(std::string data) {
    auto& instance = bulk_server::handlers::CommandsHandler::GetInstance();
    instance.AddCommands(data);
}

void ShowHelp() {
    std::cout << "Usage: bulk_server <port> <bulk_size>" << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        ShowHelp();
        return 1;
    }

    uint16_t port = 0;
    size_t bulk_size = 0;
    try {
        port = static_cast<uint16_t>(std::stoi(argv[1]));
        bulk_size = static_cast<size_t>(std::stoi(argv[2]));
    } catch (const std::runtime_error& ex) {
        std::cout << ex.what() << '\n';
        ShowHelp();
        return 1;
    }

    auto& instance = bulk_server::handlers::CommandsHandler::GetInstance();
    instance.SetBlockSize(bulk_size);

    boost::asio::io_context context;
    bulk_server::tcp::TcpServer server(context, port);
    server.RunAsync(Handle);
    context.run();

    return 0;
}
