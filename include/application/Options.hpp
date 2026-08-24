#pragma once

#include <string>

namespace app
{
// Аргументы командной строки. Разбираются в самом начале работы: от них
// зависит, уходить ли в фон и откуда брать настройки.
class Options
{
  public:
    bool daemon = false;
    std::string configPath = "";
    // Флаг вроде -h или -v уже отработал, продолжать не нужно.
    bool shouldExit = false;
    // Индекс первого аргумента, не являющегося флагом: с него начинается
    // задание для разового запуска.
    int firstArgument = 1;

    static Options parse(int argc, char** argv);

    static void printHelp(const char* program);
};
} // namespace app