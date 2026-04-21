#include "Runner.hpp"

namespace app
{
void Runner::run(int argc, char** argv)
{
    int flag_res = process_flags(argc, argv);
    if (flag_res > -1)
    {
        return;
    }

    std::string json_str;
    for (int i = optind; i < argc; ++i)
    {
        if (!json_str.empty())
            json_str += ' ';
        json_str += argv[i];
    }
    if (json_str.empty())
    {
        std::cerr << "No JSON input provided\n";
        print_help(argv[0]);
        return;
    }

    app::Task task;

    try
    {
        parser->parse(json_str, task);
        calculator.calculate(task);
        checker.check(task);
        printer->print(task);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return;
    }
}

void Runner::print_help(const char* prog)
{
    printf("Usage: %s <number1> <operation> <number2>\n", prog);
    printf("For factorial operation: <number>!\n");
    printf("  -v, --version       show version\n");
    printf("  -h, --help          show this help message\n");
}

int Runner::process_flags(int argc, char** argv)
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