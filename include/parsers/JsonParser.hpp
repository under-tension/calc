#pragma once

#include "Task.hpp"
#include "parsers/IParser.hpp"

#include <nlohmann/json.hpp>

#include <iostream>

namespace parsers
{
class JsonParser : public IParser
{
  public:
    virtual void parse(std::string input, app::Task& task) override;
};
} // namespace parsers