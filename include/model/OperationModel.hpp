#pragma once

#include <optional>
#include <string>

namespace model
{
class OperationModel
{
  public:
    std::optional<unsigned> id = std::nullopt;
    std::string operation_type = "";
    int operand1 = 0;
    int operand2 = 0;
    int result = 0;
    int status = 0;

    OperationModel() = default;

    OperationModel(const std::string& operation, int operand1, int operand2,
                   int result, int status) :
        operation_type(operation), operand1(operand1), operand2(operand2),
        result(result), status(status)
    {}

    OperationModel(std::optional<unsigned> id, const std::string& operation,
                   int operand1, int operand2, int result, int status) :
        id(id), operation_type(operation), operand1(operand1),
        operand2(operand2), result(result), status(status)
    {}
};

enum class StatusOperation
{
    OK = 0,
    NOT_ENOUGH_ARGS = 1,
    DIVISION_BY_ZERO = 2,
    NEGATIVE_DEGREE = 3,
    NEGATIVE_FACTORIAL = 4,
    NO_OPERATION = 5
};
} // namespace model