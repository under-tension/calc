#pragma once

#include "protocol/Messages.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>

#include <deque>
#include <functional>
#include <memory>
#include <string>

namespace net
{
using RequestHandler = std::function<protocol::CalculationResponse(
    const protocol::CalculationRequest&)>;

// Одно клиентское соединение. Живёт, пока не завершится последняя запущенная
// им асинхронная операция, поэтому владеет собой через shared_from_this.
class Session : public std::enable_shared_from_this<Session>
{
  private:
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf buffer_;
    RequestHandler handler_;
    std::deque<std::string> outgoing_;
    bool closing_ = false;

    void readNext();
    void handleLine(const std::string& line);
    void write(const std::string& line);
    void writeNext();

  public:
    Session(boost::asio::ip::tcp::socket socket, RequestHandler handler);

    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;

    void start();

    // Отправляет уведомление о завершении работы сервера и закрывает
    // соединение только после того, как уведомление действительно ушло.
    void notifyShutdown();

    void close();
};

using SessionPtr = std::shared_ptr<Session>;
} // namespace net