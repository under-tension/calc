#pragma once

#include "Task.hpp"

#include <iostream>

namespace printers
{
class BasePrinter
{
public:
    virtual void print(const app::Task& task) = 0;
};
}