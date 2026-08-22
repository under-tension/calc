#include "application/ServiceRunner.hpp"

#include "logger/SpdLogger.hpp"

#include <unistd.h>

#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/read_until.hpp>

#include <iostream>
#include <istream>
#include <thread>

namespace app
{
ServiceRunner::ServiceRunner(
    std::function<void(const std::string&)> onRequest) :
    // dup, чтобы закрытие дескриптора не закрывало stdin всего процесса
    input_(context_, ::dup(STDIN_FILENO)), onRequest_(std::move(onRequest)),
    signalHandler_([this] { context_.stop(); })
{}

void ServiceRunner::readNext()
{
    boost::asio::async_read_until(
        input_, buffer_, '\n',
        [this](const boost::system::error_code& error, std::size_t) {
            if (error)
            {
                return;
            }

            std::istream stream(&buffer_);
            std::string line;
            std::getline(stream, line);

            if (!line.empty())
            {
                onRequest_(line);
            }

            readNext();
        });
}

void ServiceRunner::run()
{
    // Держит цикл событий живым, даже когда ввод закрыт и ждать больше нечего,
    // кроме сигнала завершения.
    const auto guard = boost::asio::make_work_guard(context_);

    std::jthread signalThread(
        [this](std::stop_token token) { signalHandler_.run(token); });

    readNext();

    loggers::SpdLogger::GetInstance().info("Service started");

    context_.run();

    loggers::SpdLogger::GetInstance().info("Service stopped");

    std::cout << "Service stopped. Goodbye!" << std::endl;
}
} // namespace app