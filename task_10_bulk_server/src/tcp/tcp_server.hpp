#pragma once

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include <tcp/handlers.hpp>


namespace bulk_server::tcp {

class TcpServer : public boost::noncopyable {
public:
    TcpServer(boost::asio::io_context& io_context, uint16_t port);
    ~TcpServer();

    void RunAsync(DataHandler callback);

private:
    TcpServer(TcpServer&&);
    TcpServer& operator=(TcpServer&&);
    void RunAsyncImpl(DataHandler callback);

    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

} // namespace bulk_server::tcp
