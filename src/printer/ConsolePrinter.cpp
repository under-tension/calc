#include "printer/ConsolePrinter.hpp"

namespace printers
{
void ConsolePrinter::print(const model::OperationModel& operationModel)
{
    if (operationModel.operation_type == "!")
    {
        std::cout << operationModel.operand1 << "! = " << operationModel.result
                  << std::endl;
    }
    else
    {
        std::cout << operationModel.operand1 << " "
                  << operationModel.operation_type << " "
                  << operationModel.operand2 << " = " << operationModel.result
                  << std::endl;
    }
}
} // namespace printers