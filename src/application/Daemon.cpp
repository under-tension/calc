#include "application/Daemon.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>

namespace app
{
namespace
{
std::runtime_error failure(const std::string& step)
{
    return std::runtime_error("Failed to daemonize at " + step + ": " +
                              std::strerror(errno));
}

// Родитель уходит немедленно, работа продолжается в потомке.
void forkAndLeaveParent(const std::string& step)
{
    const pid_t pid = ::fork();

    if (pid < 0)
    {
        throw failure(step);
    }

    if (pid > 0)
    {
        // Завершаемся без раскрутки стека и сброса буферов: всё это
        // принадлежит потомку.
        std::_Exit(EXIT_SUCCESS);
    }
}

void redirectStandardStreams()
{
    const int nullDevice = ::open("/dev/null", O_RDWR);

    if (nullDevice < 0)
    {
        throw failure("opening /dev/null");
    }

    const bool redirected = ::dup2(nullDevice, STDIN_FILENO) >= 0 &&
                            ::dup2(nullDevice, STDOUT_FILENO) >= 0 &&
                            ::dup2(nullDevice, STDERR_FILENO) >= 0;

    if (nullDevice > STDERR_FILENO)
    {
        ::close(nullDevice);
    }

    if (!redirected)
    {
        throw failure("redirecting standard streams");
    }
}
} // namespace

void Daemon::start()
{
    forkAndLeaveParent("first fork");

    // Новая сессия: процесс перестаёт зависеть от терминала, из которого был
    // запущен, и его не убьёт закрытие этого терминала.
    if (::setsid() == static_cast<pid_t>(-1))
    {
        throw failure("setsid");
    }

    // Второе порождение: потомок не является лидером сессии и уже не может
    // получить управляющий терминал.
    forkAndLeaveParent("second fork");

    // Чтобы не удерживать смонтированный раздел, с которого запущен сервис.
    if (::chdir("/") != 0)
    {
        throw failure("chdir");
    }

    ::umask(0);

    redirectStandardStreams();
}
} // namespace app