#pragma once

#include "Calculator.hpp"
#include "Checker.hpp"
#include "IApplication.hpp"
#include "cache/HashTableCache.hpp"
#include "cache/PgCacheHydrator.hpp"
#include "db/connection/Connection.hpp"
#include "db/repository/OperationRepository.hpp"
#include "logger/ILogger.hpp"
#include "logger/SpdLogger.hpp"
#include "model/OperationModel.hpp"
#include "parser/IParser.hpp"
#include "parser/JsonParser.hpp"
#include "printer/ConsolePrinter.hpp"
#include "printer/IPrinter.hpp"
#include "protocol/Messages.hpp"

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
    std::unique_ptr<cache::ICache> cache;

    ConsoleApplication(
        std::unique_ptr<parsers::IParser> parser, Checker checker,
        Calculator calc, std::unique_ptr<printers::IPrinter> printer,
        db::connection::ConnectionPtr conn,
        std::unique_ptr<db::repository::OperationRepository> repo,
        std::unique_ptr<cache::ICache> cache) :
        parser(std::move(parser)), checker(checker), calculator(calc),
        printer(std::move(printer)), conn(std::move(conn)),
        operation_repo(std::move(repo)), cache(std::move(cache)) {};

    static loggers::ILogger& logger();
    static void error(const std::string&);
    static void warn(const std::string&);
    static void info(const std::string&);

    virtual void run(int argc, char** argv) override;

    // Обработка одного задания, пришедшего строкой: разбор -> расчёт ->
    // проверка статуса -> печать результата.
    void processOperation(const std::string& json_str);

    // Обработка задания, пришедшего по сети: возвращает результат вместо
    // печати, ошибку отдаёт в ответе.
    protocol::CalculationResponse
        processRequest(const protocol::CalculationRequest& request);

    // Общая часть обеих веток: кэш -> расчёт и сохранение при промахе.
    model::OperationModel execute(model::OperationModel operationModel);

    int process_flags(int argc, char** argv);
    void print_help(const char* prog);
};
} // namespace app
