#include "application/ConsoleApplication.hpp"
#include "application/Daemon.hpp"
#include "application/Options.hpp"
#include "application/PidFile.hpp"
#include "application/ServiceRunner.hpp"
#include "config/AppConfig.hpp"
#include "config/DotEnv.hpp"

#include <iostream>
#include <optional>

int main(int argc, char** argv)
{
    try
    {
        const app::Options options = app::Options::parse(argc, argv);

        if (options.shouldExit)
        {
            return 0;
        }

        // Настройки читаются до ухода в фон: там рабочим каталогом становится
        // корневой и относительные пути перестают работать.
        config::DotEnv::load(config::DotEnv::resolvePath(options.configPath));

        const config::AppConfig config = config::AppConfig::fromEnvironment();

        // Уход в фон — раньше потоков, сокетов и соединения с базой.
        if (options.daemon)
        {
            app::Daemon::start();
        }

        loggers::SpdLogger::Configure(config.logPath);
        loggers::SpdLogger::GetInstance();

        std::optional<app::PidFile> pidFile;
        if (options.daemon)
        {
            pidFile.emplace(config.pidPath);
        }

        auto parser = std::make_unique<parsers::JsonParser>();
        auto printer = std::make_unique<printers::ConsolePrinter>();
        auto conn = std::make_unique<db::connection::Connection>(config.dsn);
        auto repo = std::make_unique<db::repository::OperationRepository>(*conn);
        auto cache = std::make_unique<cache::HashTableCache>();

        cache::PgCacheHydrator hydrator(*repo, *cache);
        hydrator.hydrate();

        app::ConsoleApplication app(std::move(parser), app::Checker(),
                                    app::Calculator(), std::move(printer),
                                    std::move(conn), std::move(repo),
                                    std::move(cache));

        // Задание передано аргументом — выполняем разово, как раньше.
        if (!options.task.empty())
        {
            app.processOperation(options.task);

            return 0;
        }

        // Задания нет — принимаем их по сети до сигнала завершения.
        app::ServiceRunner runner(config.server(),
                                  [&app](const protocol::CalculationRequest&
                                             request)
                                  { return app.processRequest(request); });

        runner.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal: " << e.what() << std::endl;

        try
        {
            loggers::SpdLogger::GetInstance().error(e.what());
        }
        catch (const std::exception&)
        {
        }

        return 1;
    }

    return 0;
}