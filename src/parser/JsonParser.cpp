#include "parser/JsonParser.hpp"

namespace parsers
{

using json = nlohmann::json;

void JsonParser::parse(std::string input, model::OperationModel& operationModel)
{
    json json = json::parse(input);

    if (json.contains("operation") == false)
    {
        throw std::runtime_error("Operation cannot be empty");
    }

    std::string operation = json.at("operation").get<std::string>();
    if (operation.empty())
    {
        throw std::runtime_error("Operation cannot be empty");
    }

    operationModel.operation_type = operation[0];

    if (operationModel.operation_type == "!")
    {
        if (json.contains("val1") == false)
        {
            throw std::runtime_error("Values cannot be null");
        }

        operationModel.operand1 = json.at("val1").get<int>();
    }
    else
    {
        if (json.contains("val1") == false || json.contains("val2") == false)
        {
            throw std::runtime_error("Values cannot be null");
        }

        operationModel.operand1 = json.at("val1").get<int>();
        operationModel.operand2 = json.at("val2").get<int>();
    }
}
} // namespace parsers