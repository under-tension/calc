#pragma once

namespace app
{
struct Task
{
    int val1 = 0;
    int val2 = 0;
    int result = 0;
    int status = 0;
    char operation = '\0';
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