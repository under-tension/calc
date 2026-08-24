#pragma once

#include <string>

namespace app
{
// Файл с идентификатором процесса: создаётся при запуске в фоне и удаляется
// при завершении, чтобы процессом можно было управлять вручную.
class PidFile
{
  private:
    std::string path_;

  public:
    explicit PidFile(const std::string& path);
    ~PidFile();

    PidFile(const PidFile&) = delete;
    PidFile& operator=(const PidFile&) = delete;
};
} // namespace app