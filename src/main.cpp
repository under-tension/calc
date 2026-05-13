#include "application/ConsoleApplication.hpp"

int main(int argc, char** argv)
{
    app::ConsoleApplication app = app::ConsoleApplication::init();
    app.run(argc, argv);

    return 0;
}