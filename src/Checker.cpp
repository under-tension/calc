#include "Checker.hpp"

namespace app
{
void Checker::check(const model::OperationModel& operationModel)
{
    switch (operationModel.status)
    {
        case static_cast<int>(model::StatusOperation::OK):
            break;
        case static_cast<int>(model::StatusOperation::NOT_ENOUGH_ARGS):
            throw std::runtime_error("Not enough arguments");
            break;
        case static_cast<int>(model::StatusOperation::DIVISION_BY_ZERO):
            throw std::runtime_error("Error! Division by zero is not allowed");
            break;
        case static_cast<int>(model::StatusOperation::NEGATIVE_DEGREE):
            throw std::runtime_error("Error! The degree cannot be < 0");
            break;
        case static_cast<int>(model::StatusOperation::NEGATIVE_FACTORIAL):
            throw std::runtime_error("Error! The factorial cannot be < 0");
            break;
        case static_cast<int>(model::StatusOperation::NO_OPERATION):
            throw std::runtime_error("Error! No operation");
            break;
        default:
            throw std::runtime_error("Unknown error at division operation");
    }
}
} // namespace app