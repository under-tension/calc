#pragma once

#include "printer/IPrinter.hpp"

#include <iostream>

namespace printers
{
class ConsolePrinter : public IPrinter
{
  public:
    virtual void print(const model::OperationModel& operationModel) override;
};
} // namespace printers