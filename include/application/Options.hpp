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
    // Задание для разового запуска. Пустое — работаем как сервис.
    std::string task = "";

    static Options parse(int argc, char** argv);

    static void printHelp(const char* program);
};
} // namespace app
