#pragma once

#include "model/OperationModel.hpp"

namespace printers
{
class IPrinter
{
  public:
    virtual ~IPrinter() = default;
    virtual void print(const model::OperationModel& operationModel) = 0;
};
} // namespace printers