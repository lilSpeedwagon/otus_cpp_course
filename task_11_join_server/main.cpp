#include <iostream>

#include <boost/asio.hpp>

#include <tcp/server.hpp>


std::string callback(std::string data) {
    return data;
}

int main() {
    constexpr const uint16_t kPort = 8080;

    boost::asio::io_context context;
    join_server::tcp::TcpServer server(context, kPort);
    server.RunAsync(callback);
    context.run();

    return 0;
}
