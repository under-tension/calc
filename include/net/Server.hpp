#pragma once

#include "net/Session.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <string>
#include <vector>

namespace net
{
class Server
{
  private:
    boost::asio::ip::tcp::acceptor acceptor_;
    RequestHandler handler_;
    // Слабые ссылки: закрывшаяся сессия исчезает из реестра сама.
    std::vector<std::weak_ptr<Session>> sessions_;

    void accept();
    void forgetClosedSessions();

  public:
    Server(boost::asio::io_context& context, const std::string& host,
           unsigned short port, RequestHandler handler);

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    void start();

    // Перестаёт принимать подключения и рассылает активным сессиям
    // уведомление о завершении работы.
    void shutdown();

    // Фактический порт: нужен, когда сервер поднят на свободном порту.
    unsigned short port() const;
};
} // namespace net