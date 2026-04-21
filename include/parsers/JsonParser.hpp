#pragma once

#include "Task.hpp"
#include "parsers/BaseParser.hpp"

#include <iostream>
#include <nlohmann/json.hpp>

namespace parsers
{
class JsonParser : public BaseParser
{
public:
  virtual void parse(std::string input, app::Task& task) override;
};
}