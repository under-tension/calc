#include "application/PidFile.hpp"

#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <system_error>

namespace app
{
PidFile::PidFile(const std::string& path) : path_(path)
{
    const std::filesystem::path directory =
        std::filesystem::path(path_).parent_path();

    if (!directory.empty())
    {
        std::error_code ignored;
        std::filesystem::create_directories(directory, ignored);
    }

    std::ofstream file(path_, std::ios::trunc);

    if (!file.is_open())
    {
        throw std::runtime_error("Cannot write pid file: " + path_);
    }

    file << ::getpid() << '\n';
}

PidFile::~PidFile()
{
    std::error_code ignored;

    std::filesystem::remove(path_, ignored);
}
} // namespace app