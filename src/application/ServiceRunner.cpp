#include "application/ServiceRunner.hpp"

#include "logger/SpdLogger.hpp"

#include <boost/asio/post.hpp>

#include <iostream>
#include <thread>
#include <utility>

namespace app
{
ServiceRunner::ServiceRunner(const config::ServerConfig& config,
                             net::RequestHandler handler) :
    guard_(boost::asio::make_work_guard(context_)),
    server_(context_, config.host, config.port, std::move(handler)),
    signalHandler_(
        [this]
        {
            // Остановка работает с сессиями и приёмом подключений, а они
            // принадлежат рабочему потоку — передаём задачу в его цикл событий.
            boost::asio::post(context_, [this] { shutdown(); });
        })
{}

void ServiceRunner::shutdown()
{
    server_.shutdown();

    // Отпускаем цикл событий: он завершится, когда уйдут последние уведомления
    // клиентам и закроются соединения.
    guard_.reset();
}

void ServiceRunner::run()
{
    std::jthread signalThread(
        [this](std::stop_token token) { signalHandler_.run(token); });

    server_.start();

    loggers::SpdLogger::GetInstance().info("Service started");

    context_.run();

    loggers::SpdLogger::GetInstance().info("Service stopped");

    // Логгер пишет info только в файл, поэтому прощаемся отдельно.
    std::cout << "Service stopped. Goodbye!" << std::endl;
}
} // namespace app