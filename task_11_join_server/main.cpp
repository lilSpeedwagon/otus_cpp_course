#include <iostream>

#include <boost/asio.hpp>

#include <tcp/server.hpp>


std::string callback(std::string data) {
    return data;
}

int main() {
    boost::asio::io_context context;
    join_server::tcp::TcpServer server(context, 5000);
    server.RunAsync(callback);
    context.run();

    return 0;
}
