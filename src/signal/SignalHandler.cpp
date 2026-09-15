#include "signal/SignalHandler.hpp"

#include "logger/SpdLogger.hpp"

namespace signals
{
SignalHandler::SignalHandler(std::function<void()> onStop,
                             std::initializer_list<int> signals) :
    signals_(context_), onStop_(std::move(onStop))
{
    // NOLINTNEXTLINE(altera-unroll-loops)
    for (int signo : signals)
    {
        signals_.add(signo);
    }
}

void SignalHandler::run(std::stop_token token)
{
    const std::stop_callback stopCallback(token, [this] { stop(); });

    signals_.async_wait(
        [this](const boost::system::error_code& error, int signo) {
            if (error)
            {
                return;
            }

            loggers::SpdLogger::GetInstance().info(
                "Received signal " + std::to_string(signo) + ", shutting down");

            onStop_();
        });

    context_.run();
}

void SignalHandler::stop()
{
    context_.stop();
}
} // namespace signals