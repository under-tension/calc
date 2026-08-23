#include "net/Client.hpp"

#include <boost/asio/connect.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>

#include <unistd.h>

#include <iostream>
#include <istream>

namespace net
{
Client::Client(const std::string& host, unsigned short port) :
    socket_(context_),
    // dup, чтобы закрытие дескриптора не закрывало stdin всего процесса
    input_(context_, ::dup(STDIN_FILENO))
{
    boost::asio::ip::tcp::resolver resolver(context_);

    boost::asio::connect(socket_,
                         resolver.resolve(host, std::to_string(port)));
}

void Client::readServer()
{
    boost::asio::async_read_until(
        socket_, serverBuffer_, '\n',
        [this](const boost::system::error_code& error, std::size_t)
        {
            if (error)
            {
                std::cout << "Connection closed by server" << std::endl;
                stop();
                return;
            }

            std::istream stream(&serverBuffer_);
            std::string line;
            std::getline(stream, line);

            handleServerLine(line);
        });
}

void Client::handleServerLine(const std::string& line)
{
    try
    {
        const nlohmann::json json = nlohmann::json::parse(line);

        if (protocol::messageTypeOf(json) == protocol::MessageType::SHUTDOWN)
        {
            std::cout << "Server: "
                      << protocol::ShutdownNotice::fromJson(json).message
                      << std::endl;
            stop();
            return;
        }

        const protocol::CalculationResponse response =
            protocol::CalculationResponse::fromJson(json);

        if (response.failed())
        {
            std::cout << "Error: " << response.error << std::endl;
        }
        else
        {
            std::cout << "Result: " << response.result << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Malformed response: " << e.what() << std::endl;
    }

    if (pending_ > 0)
    {
        --pending_;
    }

    finishWhenDone();

    if (socket_.is_open())
    {
        readServer();
    }
}

void Client::readInput()
{
    boost::asio::async_read_until(
        input_, inputBuffer_, '\n',
        [this](const boost::system::error_code& error, std::size_t)
        {
            if (error)
            {
                // Ввод закончился: ждём ответы на уже отправленные задания.
                inputClosed_ = true;
                finishWhenDone();
                return;
            }

            std::istream stream(&inputBuffer_);
            std::string line;
            std::getline(stream, line);

            if (!line.empty())
            {
                send(line);
            }

            readInput();
        });
}

void Client::send(const std::string& line)
{
    protocol::CalculationRequest request;

    try
    {
        request =
            protocol::CalculationRequest::fromJson(nlohmann::json::parse(line));
    }
    catch (const std::exception& e)
    {
        std::cout << "Invalid request: " << e.what() << std::endl;
        return;
    }

    outgoing_ = protocol::encodeLine(request.toJson());

    boost::asio::write(socket_, boost::asio::buffer(outgoing_));

    ++pending_;
}

void Client::finishWhenDone()
{
    if (inputClosed_ && pending_ == 0)
    {
        stop();
    }
}

void Client::stop()
{
    boost::system::error_code ignored;

    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored);
    socket_.close(ignored);
    input_.close(ignored);
}

void Client::runOnce(const std::string& line)
{
    inputClosed_ = true;

    send(line);

    // Задание не отправлено — ждать ответа нечего.
    if (pending_ == 0)
    {
        stop();
        return;
    }

    readServer();

    context_.run();
}

void Client::run()
{
    readServer();
    readInput();

    context_.run();
}
} // namespace net