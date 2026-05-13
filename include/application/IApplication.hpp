#pragma once

#include "logger/ILogger.hpp"
#include "logger/SpdLogger.hpp"

namespace app
{

template <class Application>
class IApplication
{
  public:
    static loggers::ILogger& logger()
    {
        return Application::logger();
    };

    static void error(const std::string& message)
    {
        Application::error(message);
    };

    static void warn(const std::string& message)
    {
        Application::warn(message);
    };

    static void info(const std::string& message)
    {
        Application::info(message);
    };

    static Application init()
    {
        return Application::init();
    };

    virtual void run(int argc, char** argv) = 0;
};
} // namespace app
