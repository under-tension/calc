#pragma once

#include "ILogger.hpp"

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <string>

namespace loggers
{
class SpdLogger : public ILogger
{
  public:
    static SpdLogger& GetInstance();

    SpdLogger(const SpdLogger&) = delete;
    SpdLogger& operator=(const SpdLogger&) = delete;
    SpdLogger(SpdLogger&&) = delete;
    SpdLogger& operator=(SpdLogger&&) = delete;

    void error(const std::string& message) override;
    void warn(const std::string& message) override;
    void info(const std::string& message) override;

  private:
    explicit SpdLogger(std::shared_ptr<spdlog::logger> logger);

    static std::shared_ptr<spdlog::logger> CreateLogger();

    std::shared_ptr<spdlog::logger> spdLogger_;
};
} // namespace loggers