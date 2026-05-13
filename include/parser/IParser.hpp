#pragma once

#include "Task.hpp"

#include <iostream>

namespace parsers
{
class IParser
{
  public:
    virtual void parse(std::string input, app::Task& task) = 0;
};
} // namespace parsers