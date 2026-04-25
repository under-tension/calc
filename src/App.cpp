#include "App.hpp"

namespace app
{
loggers::BaseLogger& App::logger()
{
    return loggers::SpdLogger::GetInstance();
}

void App::error(const std::string& message)
{
    loggers::SpdLogger::GetInstance().error(message);
}

void App::warn(const std::string& message)
{
    loggers::SpdLogger::GetInstance().warn(message);
}

void App::info(const std::string& message)
{
    loggers::SpdLogger::GetInstance().info(message);
}
} // namespace app