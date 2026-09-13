#include "application/Options.hpp"

#include <getopt.h>

#include <cstdio>

namespace app
{
void Options::printHelp(const char* program)
{
    printf("Usage: %s [options] ['{\"val1\": 2, \"val2\": 4, \"operation\": \"+\"}']\n",
           program);
    printf("Without a task the server waits for clients on a TCP port.\n\n");
    printf("  -d, --daemon        run in background\n");
    printf("  -c, --config PATH   settings file (default: .env)\n");
    printf("  -v, --version       show version\n");
    printf("  -h, --help          show this help message\n");
}

Options Options::parse(int argc, char** argv)
{
    Options options;

    const char* short_opts = "dc:vh";
    const option long_opts[] = {{"daemon", no_argument, nullptr, 'd'},
                                {"config", required_argument, nullptr, 'c'},
                                {"version", no_argument, nullptr, 'v'},
                                {"help", no_argument, nullptr, 'h'},
                                {nullptr, 0, nullptr, 0}};

    int opt = 0;
    int long_index = 0;

    // NOLINTNEXTLINE(altera-unroll-loops)
    while ((opt = getopt_long(argc, argv, short_opts, long_opts,
                              &long_index)) != -1)
    {
        switch (opt)
        {
            case 'd':
                options.daemon = true;
                break;
            case 'c':
                options.configPath = optarg;
                break;
            case 'v':
                printf("Version %s\n", CALC_VERSION);
                options.shouldExit = true;
                return options;
            case 'h':
                printHelp(argv[0]);
                options.shouldExit = true;
                return options;
            default:
                printHelp(argv[0]);
                options.shouldExit = true;
                return options;
        }
    }

    for (int i = optind; i < argc; ++i)
    {
        if (!options.task.empty())
        {
            options.task += ' ';
        }

        options.task += argv[i];
    }

    return options;
}
} // namespace app