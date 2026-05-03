#pragma once

#include "Task.hpp"
#include "parsers/IParser.hpp"

#include <iostream>
#include <nlohmann/json.hpp>

namespace parsers
{
class JsonParser : public IParser
{
public:
  virtual void parse(std::string input, app::Task& task) override;
};
}