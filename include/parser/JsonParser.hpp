#pragma once

#include "model/OperationModel.hpp"
#include "parser/IParser.hpp"

#include <nlohmann/json.hpp>

#include <iostream>

namespace parsers
{
class JsonParser : public IParser
{
  public:
    virtual void parse(std::string input,
                       model::OperationModel& operationModel) override;
};
} // namespace parsers