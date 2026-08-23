#pragma once

#include "protocol/Messages.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/streambuf.hpp>

#include <cstddef>
#include <string>

namespace net
{
// Клиент читает задания с ввода и одновременно слушает сервер, поэтому
// замечает уведомление о завершении даже тогда, когда ничего не отправлял.
class Client
{
  private:
    boost::asio::io_context context_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::posix::stream_descriptor input_;
    boost::asio::streambuf serverBuffer_;
    boost::asio::streambuf inputBuffer_;
    std::string outgoing_;
    std::size_t pending_ = 0;
    bool inputClosed_ = false;

    void readServer();
    void handleServerLine(const std::string& line);
    void readInput();
    void send(const std::string& line);
    void finishWhenDone();
    void stop();

  public:
    Client(const std::string& host, unsigned short port);

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    // Отправляет одно задание, печатает ответ и завершается.
    void runOnce(const std::string& line);

    // Работает до конца ввода с получением всех ответов или до остановки
    // сервера.
    void run();
};
} // namespace net