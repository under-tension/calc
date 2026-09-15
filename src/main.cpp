#include "application/ConsoleApplication.hpp"

int main(int argc, char** argv)
{
    std::string dsn = "host=localhost port=5432 dbname=calc user=postgres password=postgres";

    auto parser = std::make_unique<parsers::JsonParser>();
    auto printer = std::make_unique<printers::ConsolePrinter>();
    auto conn = std::make_unique<db::connection::Connection>(dsn);
    auto repo = std::make_unique<db::repository::OperationRepository>(*conn);
    auto cache = std::make_unique<cache::HashTableCache>();

    cache::PgCacheHydrator hydrator(*repo, *cache);
    hydrator.hydrate();

    app::ConsoleApplication app(
        std::move(parser), app::Checker(), app::Calculator(),
        std::move(printer), std::move(conn), std::move(repo), std::move(cache));

    app.run(argc, argv);

    return 0;
}