#pragma once

#include <string>

namespace loggers
{
class ILogger
{
  public:
    virtual ~ILogger() = default;
    virtual void error(const std::string& message) = 0;
    virtual void warn(const std::string& message) = 0;
    virtual void info(const std::string& message) = 0;
};
} // namespace loggers