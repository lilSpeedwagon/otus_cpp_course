#pragma once

#include <boost/beast/core.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>

#include <tcp/handlers.hpp>


namespace bulk_server::tcp {

/// @brief TCP session wrapper
class TcpSession : public std::enable_shared_from_this<TcpSession>
{
public:
    explicit TcpSession(boost::asio::ip::tcp::socket&& socket,
                        const DataHandler& on_request_ready);
    void AsyncRead();
    
private:
    void OnRead(boost::beast::error_code error_code,
                std::size_t bytes_transferred);
    
    DataHandler on_request_ready_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf streambuf_;
};

} // namespace bulk_server::tcp