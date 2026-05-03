#include "application/ConsoleApplication.hpp"

using json = nlohmann::json;

int main(int argc, char** argv)
{
    app::ConsoleApplication app = app::ConsoleApplication::init();
    app.run(argc, argv);

    return 0;
}