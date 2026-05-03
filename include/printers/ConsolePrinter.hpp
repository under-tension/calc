#pragma once

#include "Task.hpp"
#include "printers/IPrinter.hpp"

#include <iostream>

namespace printers
{
class ConsolePrinter : public IPrinter
{
  public:
    virtual void print(const app::Task& task) override;
};
} // namespace printers