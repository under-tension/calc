#include "printers/ConsolePrinter.hpp"

namespace printers
{
void ConsolePrinter::print(const app::Task& task)
{
    if (task.operation == '!')
    {
        std::cout << task.val1 << "! = " << task.result << std::endl;
    }
    else
    {
        std::cout << task.val1 << " " << task.operation << " " << task.val2
                  << " = " << task.result << std::endl;
    }
}
} // namespace printers