#pragma once

#include "Task.hpp"
#include <stdexcept>

namespace app
{
class Checker
{
public:
    void check(const Task& task);
};
} // namespace app