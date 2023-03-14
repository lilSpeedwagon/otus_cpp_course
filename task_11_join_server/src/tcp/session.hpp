#pragma once

#include <functional>

#include <boost/beast/core.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>

#include <tcp/handlers.hpp>


namespace join_server::tcp {

/// @brief TCP session wrapper
class TcpSession : public std::enable_shared_from_this<TcpSession>
{
public:
    explicit TcpSession(boost::asio::ip::tcp::socket&& socket,
                        const DataHandler& on_request_ready);

    /// @brief Async wait for the next TCP connection
    void AsyncRead();

private:
    void OnRead(boost::beast::error_code error_code,
                std::size_t bytes_transferred);
    void OnWrite(boost::beast::error_code error_code, 
                 std::size_t bytes_transferred);
    void AsyncWrite(const std::string& data);
    void Close();

    DataHandler on_request_ready_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf streambuf_;
};

} // namespace join_server::tcp
