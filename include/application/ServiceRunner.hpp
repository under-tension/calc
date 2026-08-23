#pragma once

#include "config/ServerConfig.hpp"
#include "net/Server.hpp"
#include "signal/SignalHandler.hpp"

#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>

namespace app
{
class ServiceRunner
{
  private:
    boost::asio::io_context context_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
        guard_;
    net::Server server_;
    signals::SignalHandler signalHandler_;

    void shutdown();

  public:
    ServiceRunner(const config::ServerConfig& config,
                  net::RequestHandler handler);

    ServiceRunner(const ServiceRunner&) = delete;
    ServiceRunner& operator=(const ServiceRunner&) = delete;

    // Поднимает поток обработки сигналов и обслуживает клиентов до получения
    // сигнала завершения.
    void run();
};
} // namespace app