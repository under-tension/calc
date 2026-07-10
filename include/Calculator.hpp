#pragma once

#include "model/OperationModel.hpp"

#include <libmath.h>

namespace app
{
class Calculator
{
  public:
    void calculate(model::OperationModel& operationModel);
};
} // namespace app