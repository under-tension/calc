#include "config/ServerConfig.hpp"

#include <cstdlib>
#include <stdexcept>

namespace config
{
ServerConfig ServerConfig::fromEnvironment(const std::string& defaultHost)
{
    ServerConfig config;
    config.host = defaultHost;

    const char* host = std::getenv("CALC_HOST");
    if (host != nullptr && *host != '\0')
    {
        config.host = host;
    }

    const char* port = std::getenv("CALC_PORT");
    if (port != nullptr && *port != '\0')
    {
        const int value = std::stoi(port);

        if (value <= 0 || value > 65535)
        {
            throw std::runtime_error("CALC_PORT is out of range: " +
                                     std::string(port));
        }

        config.port = static_cast<unsigned short>(value);
    }

    return config;
}
} // namespace config