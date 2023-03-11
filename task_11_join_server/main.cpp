#include <iostream>

#include <boost/asio.hpp>

#include <tcp/server.hpp>
#include <utils/env.hpp>


std::string callback(std::string data) {
    return data;
}

int main() {
    const auto port = join_server::utils::GetEnv<uint16_t>("JS_PORT", 8080);

    boost::asio::io_context context;
    join_server::tcp::TcpServer server(context, port);
    server.RunAsync(callback);
    context.run();

    return 0;
}
