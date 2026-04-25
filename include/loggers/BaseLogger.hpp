#pragma once

#include <string>

namespace loggers
{
class BaseLogger
{
public:
    virtual void error(const std::string& message) = 0;
    virtual void warn(const std::string& message) = 0;
    virtual void info(const std::string& message) = 0;
};
}