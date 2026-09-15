#include "parser/JsonParser.hpp"

#include "protocol/Messages.hpp"

namespace parsers
{

using json = nlohmann::json;

void JsonParser::parse(std::string input, model::OperationModel& operationModel)
{
    const protocol::CalculationRequest request =
        protocol::CalculationRequest::fromJson(json::parse(input));

    operationModel.operation_type = request.operation;
    operationModel.operand1 = request.operand1;
    operationModel.operand2 = request.operand2;
}
} // namespace parsers