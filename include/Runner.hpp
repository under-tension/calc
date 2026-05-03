#pragma once

#include "Task.hpp"
#include "Checker.hpp"
#include "Calculator.hpp"
#include "App.hpp"
#include "parsers/IParser.hpp"
#include "parsers/JsonParser.hpp"
#include "printers/IPrinter.hpp"
#include "printers/ConsolePrinter.hpp"

#include <iostream>
#include <getopt.h>
#include <libmath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace app
{
class Runner
{
  public:
    std::unique_ptr<parsers::IParser> parser;
    Checker checker;
    Calculator calculator;
    std::unique_ptr<printers::IPrinter> printer;

    Runner(
      std::unique_ptr<parsers::IParser> p = std::make_unique<parsers::JsonParser>()
      ,Checker checker = Checker()
      ,Calculator calc = Calculator()
      ,std::unique_ptr<printers::IPrinter> printer = std::make_unique<printers::ConsolePrinter>()
    ) :
    parser(std::move(p))
    ,checker(checker)
    ,calculator(calc)
    ,printer(std::move(printer))
    {}

    ~Runner() = default;

    void run(int argc, char** argv);
    int process_flags(int argc, char** argv);
    void print_help(const char* prog);
};
} // namespace app