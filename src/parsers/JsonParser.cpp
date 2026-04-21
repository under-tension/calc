#include "parsers/JsonParser.hpp"

namespace parsers
{

using json = nlohmann::json;

void JsonParser::parse(std::string input, app::Task& task)
{
    json json = json::parse(input);

    std::string operation = json.at("operation").get<std::string>();
    if (operation.empty())
    {
        throw std::runtime_error("operation cannot be empty");
    }
    task.operation = operation[0];

    if (task.operation == '!')
    {
        task.val1 = json.at("val1").get<int>();
    }
    else
    {
        task.val1 = json.at("val1").get<int>();
        task.val2 = json.at("val2").get<int>();
    }
}
} // namespace parsers