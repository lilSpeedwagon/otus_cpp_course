#include "session.hpp"

#include <ostream>

#include <boost/asio/dispatch.hpp>

#include <logging/logger.hpp>


namespace join_server::tcp {

TcpSession::TcpSession(boost::asio::ip::tcp::socket&& socket,
                       const DataHandler& on_request_ready)
    : on_request_ready_(on_request_ready), socket_(std::move(socket)), streambuf_() {}

void TcpSession::AsyncRead() {
    boost::asio::async_read_until(
        socket_, streambuf_, '\n',
        [self=shared_from_this()] (auto error_code, auto size) {
        self->OnRead(error_code, size);
    });
}

void TcpSession::OnRead(boost::system::error_code error_code,
                        std::size_t /*bytes_transferred*/) {
    if (error_code) {   
        LOG_ERROR() << "TcpSession read error: " << error_code.message();
        return;
    }

    std::string data;
    std::istream stream(&streambuf_);
    std::getline(stream, data);

    std::string result;
    try {
        result = on_request_ready_(data);
    } catch (const std::exception& ex) {
        LOG_ERROR() << "Error during request handling: " << ex.what();
    }
    AsyncWrite(result);
}

void TcpSession::AsyncWrite(const std::string& data) {
    auto response = data + '\n';
    boost::asio::async_write(socket_, boost::asio::buffer(response),
                             [self=shared_from_this()] (auto error_code, auto size) {
        self->OnWrite(error_code, size);
    });
}

void TcpSession::OnWrite(boost::system::error_code error_code, 
                         std::size_t /*bytes_transferred*/) {
    if (error_code) {
        LOG_ERROR() << "TcpSession write error: " << error_code.message();
        return;
    }
    if (streambuf_.size() != 0) {
        AsyncRead();
    }
}

void TcpSession::Close() {
    boost::system::error_code error;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, error);
    if (error) {
        LOG_ERROR() << "TcpSession closure error: " << error.message();
    }
}

} // namespace join_server::tcp