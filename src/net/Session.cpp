#include "net/Session.hpp"

#include "logger/SpdLogger.hpp"

#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>

#include <istream>
#include <stdexcept>
#include <utility>

namespace net
{
Session::Session(boost::asio::ip::tcp::socket socket, RequestHandler handler) :
    socket_(std::move(socket)), handler_(std::move(handler))
{}

void Session::start()
{
    readNext();
}

void Session::readNext()
{
    const SessionPtr self = shared_from_this();

    boost::asio::async_read_until(
        socket_, buffer_, '\n',
        [this, self](const boost::system::error_code& error, std::size_t)
        {
            if (error)
            {
                // Клиент отключился или соединение закрыто при остановке —
                // это штатное окончание работы сессии, а не сбой.
                return;
            }

            std::istream stream(&buffer_);
            std::string line;
            std::getline(stream, line);

            if (!line.empty())
            {
                handleLine(line);
            }

            if (!closing_)
            {
                readNext();
            }
        });
}

void Session::handleLine(const std::string& line)
{
    protocol::CalculationResponse response;

    try
    {
        const nlohmann::json json = nlohmann::json::parse(line);

        if (protocol::messageTypeOf(json) != protocol::MessageType::REQUEST)
        {
            throw std::runtime_error("Unexpected message type");
        }

        response = handler_(protocol::CalculationRequest::fromJson(json));
    }
    catch (const std::exception& e)
    {
        // Некорректное задание не разрывает соединение и не роняет сервис.
        response.status = protocol::PROTOCOL_ERROR_STATUS;
        response.error = e.what();

        loggers::SpdLogger::GetInstance().warn(
            "Malformed request: " + std::string(e.what()));
    }

    write(protocol::encodeLine(response.toJson()));
}

void Session::write(const std::string& line)
{
    const bool idle = outgoing_.empty();

    outgoing_.push_back(line);

    if (idle)
    {
        writeNext();
    }
}

void Session::writeNext()
{
    const SessionPtr self = shared_from_this();

    boost::asio::async_write(
        socket_, boost::asio::buffer(outgoing_.front()),
        [this, self](const boost::system::error_code& error, std::size_t)
        {
            if (error)
            {
                close();
                return;
            }

            outgoing_.pop_front();

            if (!outgoing_.empty())
            {
                writeNext();
                return;
            }

            if (closing_)
            {
                close();
            }
        });
}

void Session::notifyShutdown()
{
    if (closing_)
    {
        return;
    }

    closing_ = true;

    write(protocol::encodeLine(protocol::ShutdownNotice().toJson()));
}

void Session::close()
{
    boost::system::error_code ignored;

    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored);
    socket_.close(ignored);
}
} // namespace net