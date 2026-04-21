#include "Calculator.hpp"

namespace app
{
void Calculator::calculate(Task& task)
{
    switch (task.operation)
    {
        case '+':
            task.status = libmath::add(task.val1, task.val2, task.result);
            break;
        case '-':
            task.status = libmath::sub(task.val1, task.val2, task.result);
            break;
        case '*':
            task.status =
                libmath::multiplication(task.val1, task.val2, task.result);
            break;
        case '/':
            task.status = libmath::division(task.val1, task.val2, task.result);
            break;
        case '^':
            task.status = libmath::power(task.val1, task.val2, task.result);
            break;
        case '!':
            task.status = libmath::factorial(task.val1, task.result);
            break;
        default:
            task.status = 5;
    }
}
} // namespace app