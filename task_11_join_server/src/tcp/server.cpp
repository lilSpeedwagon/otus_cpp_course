#include "server.hpp"

#include <logging/logger.hpp>
#include <tcp/session.hpp>


namespace join_server::tcp {

TcpServer::TcpServer(boost::asio::io_context& io_context, uint16_t port)
    : io_context_(io_context),
      acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {}

TcpServer::~TcpServer() {}

void TcpServer::RunAsync(DataHandler callback) {
    LOG_INFO() << "Running server on port " << acceptor_.local_endpoint().port();
    RunAsyncImpl(callback);
}

void TcpServer::RunAsyncImpl(DataHandler callback) {
    auto socket_ptr = std::make_shared<boost::asio::ip::tcp::socket>(io_context_);
    acceptor_.async_accept(*socket_ptr, [this, socket_ptr, callback](boost::system::error_code error) {
        if (error) {
            LOG_ERROR() << "Cannot accept new connection: " << error.message();
            return;
        }

        auto session_ptr = std::make_shared<tcp::TcpSession>(std::move(*socket_ptr), callback);
        session_ptr->AsyncRead();
        this->RunAsyncImpl(callback);
    });
}

} // namespace join_server::tcp
