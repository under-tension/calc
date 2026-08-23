#include "logger/SpdLogger.hpp"

namespace loggers
{
void SpdLogger::error(const std::string& message)
{
    spdLogger_->error(message);
}
void SpdLogger::warn(const std::string& message)
{
    spdLogger_->warn(message);
}
void SpdLogger::info(const std::string& message)
{
    spdLogger_->info(message);
}

std::shared_ptr<spdlog::logger> SpdLogger::CreateLogger()
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::warn);

    const unsigned rotstion_limit = 3;
    const unsigned file_log_size = 1024 * 1024 * 5; // 5 MB
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        "logs/app.log", file_log_size, rotstion_limit);

    file_sink->set_level(spdlog::level::trace);

    auto spdLogger = std::make_shared<spdlog::logger>(
        "multi_sink", spdlog::sinks_init_list{console_sink, file_sink});

    // Без этого записи копятся в буфере и попадают в файл только при
    // остановке сервиса — ошибки нужно видеть сразу.
    spdLogger->flush_on(spdlog::level::warn);

    return spdLogger;
}

SpdLogger::SpdLogger(std::shared_ptr<spdlog::logger> logger) :
    spdLogger_(std::move(logger))
{}

SpdLogger& SpdLogger::GetInstance()
{
    static SpdLogger instance(CreateLogger());
    return instance;
}
} // namespace loggers