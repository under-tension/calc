#include "Checker.hpp"

namespace app
{
void Checker::check(const Task& task)
{
    switch (task.status)
    {
        case 0:
            break;
        case 1:
            throw std::runtime_error("Not enough arguments");
            break;
        case 2:
            throw std::runtime_error("Error! Division by zero is not allowed");
            break;
        case 3:
            throw std::runtime_error("Error! The degree cannot be < 0");
            break;
        case 4:
            throw std::runtime_error("Error! The factorial cannot be < 0");
            break;
        case 5:
            throw std::runtime_error("Error! No operation");
            break;
        default:
            throw std::runtime_error("Unknown error at division operation");
    }
}
} // namespace app