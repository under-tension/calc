#pragma once

#include "Task.hpp"
#include "Checker.hpp"
#include "Calculator.hpp"
#include "App.hpp"
#include "parsers/BaseParser.hpp"
#include "parsers/JsonParser.hpp"
#include "printers/BasePrinter.hpp"
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
    parsers::BaseParser* parser;
    Checker checker;
    Calculator calculator;
    printers::BasePrinter* printer;

    Runner(
      parsers::BaseParser* p = new parsers::JsonParser()
      ,Checker checker = Checker()
      ,Calculator calc = Calculator()
      ,printers::BasePrinter* printer = new printers::ConsolePrinter()
    ) :
    parser(p)
    ,checker(checker)
    ,calculator(calc)
    ,printer(printer)
    {}

    ~Runner()
    {
        delete parser;
        delete printer;
    }

    void run(int argc, char** argv);
    int process_flags(int argc, char** argv);
    void print_help(const char* prog);
};
} // namespace app