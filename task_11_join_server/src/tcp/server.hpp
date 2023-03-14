#pragma once

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include <tcp/handlers.hpp>


namespace join_server::tcp {

/// @brief Simple async TCP server based on boost/asio 
class TcpServer : public boost::noncopyable {
public:
    TcpServer(boost::asio::io_context& io_context, uint16_t port);
    ~TcpServer();

    /// @brief Async wait for connections
    /// @param callback TCP connection handler
    void RunAsync(DataHandler callback);

private:
    TcpServer(TcpServer&&);
    TcpServer& operator=(TcpServer&&);
    void RunAsyncImpl(DataHandler callback);

    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

} // namespace join_server::tcp
