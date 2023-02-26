#include <iostream>

#include <boost/asio.hpp>

#include <tcp/tcp_server.hpp>


void Handle(std::string data) {
    std::cout << data << '\n';
}

int main() {
    static const uint16_t kPort = 5000;

    boost::asio::io_context context;
    bulk_server::tcp::TcpServer server(context, kPort);
    server.RunAsync(Handle);
    context.run();

    return 0;
}
