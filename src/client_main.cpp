#include "config/ServerConfig.hpp"
#include "net/Client.hpp"

#include <iostream>

int main(int argc, char** argv)
{
    try
    {
        const config::ServerConfig config =
            config::ServerConfig::fromEnvironment("127.0.0.1");

        net::Client client(config.host, config.port);

        // Задание передано аргументом — отправляем его и выходим.
        if (argc > 1)
        {
            std::string line;

            // NOLINTNEXTLINE(altera-unroll-loops)
            for (int i = 1; i < argc; ++i)
            {
                if (!line.empty())
                {
                    line += ' ';
                }

                line += argv[i];
            }

            client.runOnce(line);

            return 0;
        }

        // Аргументов нет — читаем задания с ввода, пока работает сервер.
        client.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Client error: " << e.what() << std::endl;

        return 1;
    }

    return 0;
}