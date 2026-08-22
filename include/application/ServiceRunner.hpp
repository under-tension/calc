#pragma once

#include "signal/SignalHandler.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/streambuf.hpp>

#include <functional>
#include <string>

namespace app
{
class ServiceRunner
{
  private:
    boost::asio::io_context context_;
    boost::asio::posix::stream_descriptor input_;
    boost::asio::streambuf buffer_;
    std::function<void(const std::string&)> onRequest_;
    signals::SignalHandler signalHandler_;

    void readNext();

  public:
    explicit ServiceRunner(std::function<void(const std::string&)> onRequest);

    ServiceRunner(const ServiceRunner&) = delete;
    ServiceRunner& operator=(const ServiceRunner&) = delete;

    // Поднимает поток обработки сигналов и работает до получения сигнала
    // завершения, обрабатывая поступающие задания.
    void run();
};
} // namespace app