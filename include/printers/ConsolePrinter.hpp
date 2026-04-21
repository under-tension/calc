#pragma once

#include "Task.hpp"
#include "printers/BasePrinter.hpp"

#include <iostream>

namespace printers
{
class ConsolePrinter : public BasePrinter
{
public:
    virtual void print(const app::Task& task) override;
};
}