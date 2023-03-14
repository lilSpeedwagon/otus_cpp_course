#include <iostream>

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#include <db/init.hpp>
#include <logging/logger.hpp>
#include <handlers.hpp>
#include <tcp/server.hpp>


std::string HandleCommands(const std::string& data) {
    if (data.empty()) {
        throw std::runtime_error("Empty input is not allowed");
    }
    
    std::vector<std::string> tokens;
    boost::split(tokens, data, boost::is_any_of(" "));
    auto command = tokens.front();

    if (command == "INSERT") {
        if (tokens.size() != 4) {
            throw std::runtime_error("Invalid arguments number");
        }

        const auto table_name = tokens[1];
        const auto id = tokens[2];
        const auto name = tokens[3];
        return join_server::handlers::HandleInsert(table_name, id, name);
    } else if (command == "TRUNCATE") {
        if (tokens.size() != 2) {
            throw std::runtime_error("Invalid arguments number");
        }

        const auto table_name = tokens[1];
        return join_server::handlers::HandleTruncate(table_name);
    } else if (command == "INTERSECT") {
        if (tokens.size() != 1) {
            throw std::runtime_error("Invalid arguments number");
        }

        return join_server::handlers::HandleIntersetion();
    } else if (command == "SYMMETRIC_DIFFERENCE") {
        if (tokens.size() != 1) {
            throw std::runtime_error("Invalid arguments number");
        }

        return join_server::handlers::HandleDifference();
    }

    throw std::runtime_error("Unknown command");
}

std::string HandleRequest(std::string data) {
    LOG_INFO() << "Request: " << data;
    std::string response;
    try {
        response = HandleCommands(data);
    } catch (const std::exception& ex) {
        response = std::string("ERR ") + ex.what();
    }
    LOG_INFO() << "Response: " << response;
    return response;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "invalid args count\n";
        std::cout << "usage: join_server <port>\n";
        return 1;
    }

    const auto port = static_cast<uint16_t>(std::atoi(argv[1]));

    join_server::db::Init();

    boost::asio::io_context context;
    join_server::tcp::TcpServer server(context, port);
    server.RunAsync(HandleRequest);
    context.run();

    return 0;
}
