#pragma once

#include <iostream>
#include "Task.hpp"

namespace parsers
{
class BaseParser
{
public:
  virtual void parse(std::string input, app::Task& task) = 0;
};
}