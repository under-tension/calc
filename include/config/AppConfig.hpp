#pragma once

#include "config/ServerConfig.hpp"

#include <string>

namespace config
{
// Все настройки сервера. Значения по умолчанию подобраны так, чтобы запуск из
// каталога сборки работал без файла настроек вовсе.
class AppConfig
{
  public:
    std::string host = "0.0.0.0";
    unsigned short port = 9000;
    std::string dsn =
        "host=localhost port=5432 dbname=calc user=postgres password=postgres";
    std::string logPath = "logs/app.log";
    std::string pidPath = "calc.pid";

    ServerConfig server() const
    {
        return ServerConfig(host, port);
    }

    // Читает CALC_HOST, CALC_PORT, CALC_DSN, CALC_LOG_PATH, CALC_PID_PATH.
    static AppConfig fromEnvironment();
};
} // namespace config