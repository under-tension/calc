#include "config/DotEnv.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string_view>

namespace config
{
namespace
{
constexpr std::string_view SYSTEM_PATH = "/etc/calc/calc.env";
constexpr std::string_view FILE_NAME = ".env";

std::string trim(const std::string& value)
{
    const std::string spaces = " \t\r\n";

    const size_t first = value.find_first_not_of(spaces);
    if (first == std::string::npos)
    {
        return "";
    }

    const size_t last = value.find_last_not_of(spaces);

    return value.substr(first, last - first + 1);
}

std::string unquote(const std::string& value)
{
    if (value.size() < 2)
    {
        return value;
    }

    const char first = value.front();

    if ((first == '"' || first == '\'') && value.back() == first)
    {
        return value.substr(1, value.size() - 2);
    }

    return value;
}

// Каталог исполняемого файла: нужен, чтобы запуск из другого места всё равно
// находил файл настроек рядом с бинарником.
std::filesystem::path executableDirectory()
{
    std::error_code error;
    const std::filesystem::path executable =
        std::filesystem::read_symlink("/proc/self/exe", error);

    if (error)
    {
        return {};
    }

    return executable.parent_path();
}

bool isReadableFile(const std::filesystem::path& path)
{
    std::error_code error;

    return std::filesystem::is_regular_file(path, error);
}
} // namespace

void DotEnv::load(const std::string& path)
{
    if (path.empty())
    {
        return;
    }

    std::ifstream file(path);
    if (!file.is_open())
    {
        return;
    }

    std::string line;

    // NOLINTNEXTLINE(altera-unroll-loops)
    while (std::getline(file, line))
    {
        const std::string trimmed = trim(line);

        if (trimmed.empty() || trimmed.front() == '#')
        {
            continue;
        }

        const size_t separator = trimmed.find('=');
        if (separator == std::string::npos)
        {
            continue;
        }

        const std::string name = trim(trimmed.substr(0, separator));
        if (name.empty())
        {
            continue;
        }

        const std::string value =
            unquote(trim(trimmed.substr(separator + 1)));

        // Последний аргумент 0: значение из окружения имеет приоритет.
        ::setenv(name.c_str(), value.c_str(), 0);
    }
}

std::string DotEnv::resolvePath(const std::string& explicitPath)
{
    if (!explicitPath.empty())
    {
        return explicitPath;
    }

    const std::filesystem::path local =
        std::filesystem::path(".") / FILE_NAME;

    if (isReadableFile(local))
    {
        return local.string();
    }

    const std::filesystem::path executableDir = executableDirectory();
    if (!executableDir.empty())
    {
        const std::filesystem::path nearby = executableDir / FILE_NAME;

        if (isReadableFile(nearby))
        {
            return nearby.string();
        }
    }

    const std::filesystem::path system(SYSTEM_PATH);
    if (isReadableFile(system))
    {
        return system.string();
    }

    return "";
}
} // namespace config