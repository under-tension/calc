#include "application/ConsoleApplication.hpp"

namespace app
{
loggers::ILogger& ConsoleApplication::logger()
{
    return loggers::SpdLogger::GetInstance();
}

void ConsoleApplication::error(const std::string& message)
{
    loggers::SpdLogger::GetInstance().error(message);
}

void ConsoleApplication::warn(const std::string& message)
{
    loggers::SpdLogger::GetInstance().warn(message);
}

void ConsoleApplication::info(const std::string& message)
{
    loggers::SpdLogger::GetInstance().info(message);
}

void ConsoleApplication::run(int argc, char** argv)
{
    try
    {
        std::string json_str;

        // NOLINTNEXTLINE(altera-unroll-loops)
        for (int i = optind; i < argc; ++i)
        {
            if (!json_str.empty())
            {
                json_str += ' ';
            }

            json_str += argv[i];
        }

        if (json_str.empty())
        {
            ConsoleApplication::error("No JSON input provided");
            Options::printHelp(argv[0]);
            return;
        }

        processOperation(json_str);
    }
    catch (const std::exception& e)
    {
        ConsoleApplication::error(e.what());
    }
}

model::OperationModel
    ConsoleApplication::execute(model::OperationModel operationModel)
{
    std::optional<model::OperationModel> cacheRes = cache->get(operationModel);

    if (cacheRes.has_value())
    {
        operationModel.result = cacheRes.value().result;
        operationModel.status = cacheRes.value().status;

        return operationModel;
    }

    calculator.calculate(operationModel);
    operation_repo->insert(operationModel);
    cache->set(operationModel, operationModel);

    return operationModel;
}

void ConsoleApplication::processOperation(const std::string& json_str)
{
    try
    {
        model::OperationModel operationModel;

        parser->parse(json_str, operationModel);

        operationModel = execute(operationModel);

        checker.check(operationModel);
        printer->print(operationModel);
    }
    catch (const std::exception& e)
    {
        ConsoleApplication::error(e.what());
    }
}

protocol::CalculationResponse
    ConsoleApplication::processRequest(const protocol::CalculationRequest& request)
{
    protocol::CalculationResponse response;

    try
    {
        model::OperationModel operationModel;
        operationModel.operation_type = request.operation;
        operationModel.operand1 = request.operand1;
        operationModel.operand2 = request.operand2;

        operationModel = execute(operationModel);

        response.result = operationModel.result;
        response.status = operationModel.status;

        // Превращает ненулевой статус в понятное сообщение об ошибке.
        checker.check(operationModel);
    }
    catch (const std::exception& e)
    {
        response.error = e.what();

        ConsoleApplication::error("Request " + request.describe() +
                                  " failed: " + e.what());
    }

    return response;
}

} // namespace app