#pragma once

#include "loggers/ILogger.hpp"
#include "loggers/SpdLogger.hpp"

namespace app
{
class App
{
public:
    static loggers::ILogger& logger();
    static void error(const std::string&);
    static void warn(const std::string&);
    static void info(const std::string&);
};
} // namespace app
