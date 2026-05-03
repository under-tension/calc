#pragma once

#include "Task.hpp"

#include <iostream>

namespace printers
{
class IPrinter
{
  public:
    virtual void print(const app::Task& task) = 0;
};
} // namespace printers