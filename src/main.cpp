#include "Runner.hpp"

using json = nlohmann::json;

int main(int argc, char** argv)
{
    app::Runner runner = app::Runner();
    runner.run(argc, argv);

    return 0;
}