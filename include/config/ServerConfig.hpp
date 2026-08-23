#pragma once

#include <string>

namespace config
{
class ServerConfig
{
  public:
    std::string host = "0.0.0.0";
    unsigned short port = 9000;

    ServerConfig() = default;

    ServerConfig(const std::string& host, unsigned short port) :
        host(host), port(port)
    {}

    // Читает CALC_HOST и CALC_PORT; отсутствующие значения остаются
    // умолчаниями.
    static ServerConfig fromEnvironment(const std::string& defaultHost);
};
} // namespace config