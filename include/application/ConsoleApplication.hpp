#pragma once

#include "Calculator.hpp"
#include "Checker.hpp"
#include "IApplication.hpp"
#include "Task.hpp"
#include "db/connection/Connection.hpp"
#include "db/repository/OperationRepository.hpp"
#include "logger/ILogger.hpp"
#include "logger/SpdLogger.hpp"
#include "parser/IParser.hpp"
#include "parser/JsonParser.hpp"
#include "printer/ConsolePrinter.hpp"
#include "printer/IPrinter.hpp"

#include <getopt.h>

namespace app
{
class ConsoleApplication : public IApplication<ConsoleApplication>
{
  public:
    std::unique_ptr<parsers::IParser> parser;
    Checker checker;
    Calculator calculator;
    std::unique_ptr<printers::IPrinter> printer;
    db::connection::ConnectionPtr conn;
    std::unique_ptr<db::repository::OperationRepository> operation_repo;

    ConsoleApplication(std::unique_ptr<parsers::IParser> p =
                           std::make_unique<parsers::JsonParser>(),
                       Checker checker = {}, Calculator calc = {},
                       std::unique_ptr<printers::IPrinter> printer =
                           std::make_unique<printers::ConsolePrinter>());

    static loggers::ILogger& logger();
    static void error(const std::string&);
    static void warn(const std::string&);
    static void info(const std::string&);

    static ConsoleApplication init();
    virtual void run(int argc, char** argv) override;

    int process_flags(int argc, char** argv);
    void print_help(const char* prog);
};
} // namespace app
