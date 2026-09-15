#include "Calculator.hpp"

namespace app
{
void Calculator::calculate(model::OperationModel& operationModel)
{
    switch (operationModel.operation_type[0])
    {
        case '+':
            operationModel.status =
                libmath::add(operationModel.operand1, operationModel.operand2,
                             operationModel.result);
            break;
        case '-':
            operationModel.status =
                libmath::sub(operationModel.operand1, operationModel.operand2,
                             operationModel.result);
            break;
        case '*':
            operationModel.status = libmath::multiplication(
                operationModel.operand1, operationModel.operand2,
                operationModel.result);
            break;
        case '/':
            operationModel.status = libmath::division(operationModel.operand1,
                                                      operationModel.operand2,
                                                      operationModel.result);
            break;
        case '^':
            operationModel.status =
                libmath::power(operationModel.operand1, operationModel.operand2,
                               operationModel.result);
            break;
        case '!':
            operationModel.status = libmath::factorial(operationModel.operand1,
                                                       operationModel.result);
            break;
        default:
            operationModel.status = 5;
    }
}
} // namespace app