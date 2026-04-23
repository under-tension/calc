#include "Checker.hpp"

namespace app
{
void Checker::check(const Task& task)
{
    switch (task.status)
    {
        case static_cast<int>(StatusTask::OK):
            break;
        case static_cast<int>(StatusTask::NOT_ENOUGH_ARGS):
            throw std::runtime_error("Not enough arguments");
            break;
        case static_cast<int>(StatusTask::DIVISION_BY_ZERO):
            throw std::runtime_error("Error! Division by zero is not allowed");
            break;
        case static_cast<int>(StatusTask::NEGATIVE_DEGREE):
            throw std::runtime_error("Error! The degree cannot be < 0");
            break;
        case static_cast<int>(StatusTask::NEGATIVE_FACTORIAL):
            throw std::runtime_error("Error! The factorial cannot be < 0");
            break;
        case static_cast<int>(StatusTask::NO_OPERATION):
            throw std::runtime_error("Error! No operation");
            break;
        default:
            throw std::runtime_error("Unknown error at division operation");
    }
}
} // namespace app