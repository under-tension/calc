#include "config/AppConfig.hpp"

#include <cstdlib>
#include <filesystem>
#include <system_error>

namespace config
{
namespace
{
std::string valueOr(const char* name, const std::string& fallback)
{
    const char* value = std::getenv(name);

    if (value == nullptr || *value == '\0')
    {
        return fallback;
    }

    return value;
}

// Настройки читаются до ухода в фон, где рабочим каталогом становится
// корневой: относительный путь после этого указывал бы совсем не туда.
std::string absolutePath(const std::string& path)
{
    std::error_code error;
    const std::filesystem::path resolved =
        std::filesystem::absolute(path, error);

    if (error)
    {
        return path;
    }

    return resolved.string();
}
} // namespace

AppConfig AppConfig::fromEnvironment()
{
    AppConfig config;

    const ServerConfig server = ServerConfig::fromEnvironment(config.host);
    config.host = server.host;
    config.port = server.port;

    config.dsn = valueOr("CALC_DSN", config.dsn);
    config.logPath = absolutePath(valueOr("CALC_LOG_PATH", config.logPath));
    config.pidPath = absolutePath(valueOr("CALC_PID_PATH", config.pidPath));

    return config;
}
} // namespace config