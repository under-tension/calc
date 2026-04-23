#pragma once

namespace app
{
struct Task
{
    int val1;
    int val2;
    int result;
    int status;
    char operation;
};

enum class StatusTask
{
    OK = 0,
    NOT_ENOUGH_ARGS = 1,
    DIVISION_BY_ZERO = 2,
    NEGATIVE_DEGREE = 3,
    NEGATIVE_FACTORIAL = 4,
    NO_OPERATION = 5
};
} // namespace app