#pragma once

#include <postgresql/libpq-fe.h>

#include <memory>
#include <stdexcept>
#include <string>

namespace db::connection
{
class Connection
{
  private:
    PGconn* conn;

  public:
    Connection(const std::string& conninfo)
    {
        conn = PQconnectdb(conninfo.c_str());

        if (PQstatus(conn) != CONNECTION_OK)
        {
            std::string err_msg = PQerrorMessage(conn);
            conn = nullptr;
            throw std::runtime_error("Connection to database failed: " +
                                     err_msg);
        }
    }

    ~Connection()
    {
        if (conn)
        {
            PQfinish(conn);
        }
    }

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    Connection(Connection&& other) noexcept
    {
        conn = other.conn;
        other.conn = nullptr;
    }

    Connection& operator=(Connection&& other) noexcept
    {
        if (this != &other)
        {
            PQfinish(conn);
            conn = other.conn;
            other.conn = nullptr;
        }

        return *this;
    }

    PGconn* get() const
    {
        return conn;
    }

    PGconn* operator->() const
    {
        return conn;
    }
};

using ConnectionPtr = std::unique_ptr<Connection>;
} // namespace db::connection