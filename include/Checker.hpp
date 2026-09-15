#pragma once

#include "model/OperationModel.hpp"

#include <stdexcept>

namespace app
{
class Checker
{
  public:
    void check(const model::OperationModel& operationModel);
};
} // namespace app