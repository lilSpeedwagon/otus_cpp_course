#include <iostream>

#include <boost/asio.hpp>

#include <db/init.hpp>
#include <tcp/server.hpp>
#include <utils/env.hpp>


std::string callback(std::string data) {
    return data;
}

int main() {
    constexpr const uint16_t kPort = 8080;

    join_server::db::Init();

    boost::asio::io_context context;
    join_server::tcp::TcpServer server(context, kPort);
    server.RunAsync(callback);
    context.run();

    return 0;
}
