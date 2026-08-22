#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

#include <csignal>
#include <functional>
#include <initializer_list>
#include <stop_token>

namespace signals
{
class SignalHandler
{
  private:
    boost::asio::io_context context_;
    boost::asio::signal_set signals_;
    std::function<void()> onStop_;

  public:
    SignalHandler(std::function<void()> onStop,
                  std::initializer_list<int> signals = {SIGTERM, SIGINT});

    SignalHandler(const SignalHandler&) = delete;
    SignalHandler& operator=(const SignalHandler&) = delete;

    // Тело потока обработки сигналов: блокируется до прихода сигнала,
    // после чего вызывает onStop. Возвращает управление также при запросе
    // остановки через token.
    void run(std::stop_token token);

    void stop();
};
} // namespace signals