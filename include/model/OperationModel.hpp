#pragma once

#include <string>

namespace model
{
class OperationModel
{
  public:
    unsigned id;
    std::string operation_type;
    int operand1;
    int operand2;
    int result;
    int status;

    OperationModel(unsigned id, const std::string& operation, int operand1,
                   int operand2, int result, int status) :
        id(id), operation_type(operation), operand1(operand1),
        operand2(operand2), result(result), status(status)
    {}

    OperationModel(const std::string& operation, int operand1, int operand2,
                   int result, int status) :
        operation_type(operation), operand1(operand1), operand2(operand2),
        result(result), status(status)
    {}
};
} // namespace model