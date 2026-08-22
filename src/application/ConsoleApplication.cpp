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
        int flag_res = process_flags(argc, argv);
        if (flag_res > -1)
        {
            return;
        }

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
            print_help(argv[0]);
            return;
        }

        processOperation(json_str);
    }
    catch (const std::exception& e)
    {
        ConsoleApplication::error(e.what());
    }
}

void ConsoleApplication::processOperation(const std::string& json_str)
{
    try
    {
        model::OperationModel operationModel;

        parser->parse(json_str, operationModel);

        std::optional<model::OperationModel> cacheRes =
            cache->get(operationModel);

        if (cacheRes.has_value())
        {
            operationModel.result = cacheRes.value().result;
            operationModel.status = cacheRes.value().status;
        }
        else
        {
            calculator.calculate(operationModel);
            operation_repo->insert(operationModel);
            cache->set(operationModel, operationModel);
        }

        checker.check(operationModel);
        printer->print(operationModel);
    }
    catch (const std::exception& e)
    {
        ConsoleApplication::error(e.what());
    }
}

void ConsoleApplication::print_help(const char* prog)
{
    printf("For factorial operation: <number>!\n");
    printf("  -v, --version       show version\n");
    printf("  -h, --help          show this help message\n");
}

int ConsoleApplication::process_flags(int argc, char** argv)
{
    const char* short_opts = "vh";
    const option long_opts[] = {{"version", no_argument, nullptr, 'v'},
                                {"help", no_argument, nullptr, 'h'},
                                {nullptr, 0, nullptr, 0}};

    int opt;
    int long_index = 0;

    // NOLINTNEXTLINE(altera-unroll-loops)
    while ((opt = getopt_long(argc, argv, short_opts, long_opts,
                              &long_index)) != -1)
    {
        switch (opt)
        {
            case 'v':
                printf("Version 1.0\n");
                return 0;
            case 'h':
                print_help(argv[0]);
                return 0;
            default:
                printf("Unknown flag %c\n", opt);
                return -1;
        }
    }

    return -1;
}
} // namespace app