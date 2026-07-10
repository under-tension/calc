#pragma once

#include "model/OperationModel.hpp"

#include <iostream>

namespace parsers
{
class IParser
{
  public:
    virtual ~IParser() = default;
    virtual void parse(std::string input,
                       model::OperationModel& operationModel) = 0;
};
} // namespace parsers