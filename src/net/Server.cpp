#include "net/Server.hpp"

#include "logger/SpdLogger.hpp"

#include <algorithm>
#include <utility>

namespace net
{
Server::Server(boost::asio::io_context& context, const std::string& host,
               unsigned short port, RequestHandler handler) :
    acceptor_(context, boost::asio::ip::tcp::endpoint(
                           boost::asio::ip::make_address(host), port)),
    handler_(std::move(handler))
{}

void Server::start()
{
    loggers::SpdLogger::GetInstance().info("Listening on port " +
                                           std::to_string(port()));

    accept();
}

void Server::accept()
{
    acceptor_.async_accept(
        [this](const boost::system::error_code& error,
               boost::asio::ip::tcp::socket socket)
        {
            if (error)
            {
                // Приём остановлен: акцептор закрыт при завершении работы.
                return;
            }

            const SessionPtr session =
                std::make_shared<Session>(std::move(socket), handler_);

            forgetClosedSessions();
            sessions_.push_back(session);

            session->start();

            accept();
        });
}

void Server::forgetClosedSessions()
{
    const auto closed = std::remove_if(
        sessions_.begin(), sessions_.end(),
        [](const std::weak_ptr<Session>& session) { return session.expired(); });

    sessions_.erase(closed, sessions_.end());
}

void Server::shutdown()
{
    boost::system::error_code ignored;
    acceptor_.close(ignored);

    // NOLINTNEXTLINE(altera-unroll-loops)
    for (const std::weak_ptr<Session>& weak : sessions_)
    {
        if (const SessionPtr session = weak.lock())
        {
            session->notifyShutdown();
        }
    }

    sessions_.clear();
}

unsigned short Server::port() const
{
    return acceptor_.local_endpoint().port();
}
} // namespace net