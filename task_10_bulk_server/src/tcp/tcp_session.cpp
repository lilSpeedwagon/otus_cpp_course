#include "tcp_session.hpp"

#include <sstream>

#include <boost/asio/dispatch.hpp>

#include <logging/logger.hpp>


namespace bulk_server::tcp {

TcpSession::TcpSession(boost::asio::ip::tcp::socket&& socket,
                       const DataHandler& on_request_ready)
    : on_request_ready_{on_request_ready}, socket_{std::move(socket)}, streambuf_{} {}

void TcpSession::AsyncRead() {
    boost::asio::async_read_until(
        socket_, streambuf_, '\n',
        [self=shared_from_this()] (auto error_code, auto size) {
        self->OnRead(error_code, size);
    });
}

void TcpSession::OnRead(boost::beast::error_code error_code,
                        std::size_t /*bytes_transferred*/) {
    if (error_code) {
        LOG_ERROR() << "TcpSession read error: " << error_code.message();
        return;
    }

    std::string data((std::istreambuf_iterator<char>(&streambuf_)), std::istreambuf_iterator<char>());
    on_request_ready_(data);
}

} // namespace bulk_server::tcp
