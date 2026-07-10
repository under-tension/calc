#pragma once

#include <postgresql/libpq-fe.h>

#include <memory>
#include <stdexcept>
#include <string>

namespace db::connection
{
struct PGconnDeleter
{
    void operator()(PGconn* conn) const noexcept
    {
        PQfinish(conn);
    }
};

struct PGresultDeleter
{
    void operator()(PGresult* result) const noexcept
    {
        PQclear(result);
    }
};

using PGconnPtr = std::unique_ptr<PGconn, PGconnDeleter>;
using PGresultPtr = std::unique_ptr<PGresult, PGresultDeleter>;

class Connection
{
  private:
    PGconnPtr conn_;

  public:
    Connection(const std::string& conninfo) :
        conn_(PQconnectdb(conninfo.c_str()))
    {
        if (PQstatus(conn_.get()) != CONNECTION_OK)
        {
            std::string err_msg = PQerrorMessage(conn_.get());
            throw std::runtime_error("Connection to database failed: " +
                                     err_msg);
        }
    }

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    Connection(Connection&&) noexcept = default;
    Connection& operator=(Connection&&) noexcept = default;

    PGconn* get() const
    {
        return conn_.get();
    }

    PGconn* operator->() const
    {
        return conn_.get();
    }
};

class QueryResult
{
  private:
    PGresultPtr result_;

  public:
    explicit QueryResult(PGresult* res) : result_(res)
    {
        if (!result_)
            throw std::runtime_error("Null PGresult provided");
    }

    QueryResult(const QueryResult&) = delete;
    QueryResult& operator=(const QueryResult&) = delete;

    QueryResult(QueryResult&&) noexcept = default;
    QueryResult& operator=(QueryResult&&) noexcept = default;

    PGresult* get() const
    {
        return result_.get();
    }

    PGresult* operator->() const
    {
        return result_.get();
    }

    int rowCount() const
    {
        return PQntuples(result_.get());
    }

    int columnCount() const
    {
        return PQnfields(result_.get());
    }

    bool isNull(int row, int col) const
    {
        return PQgetisnull(result_.get(), row, col);
    }

    std::string getString(int row, int col) const
    {
        if (isNull(row, col))
        {
            return "";
        }

        return PQgetvalue(result_.get(), row, col);
    }

    int getInt(int row, int col) const
    {
        if (isNull(row, col))
        {
            throw std::runtime_error("Cannot get int from NULL column");
        }

        return std::stoi(PQgetvalue(result_.get(), row, col));
    }

    unsigned getUInt(int row, int col) const
    {
        if (isNull(row, col))
            throw std::runtime_error("NULL column");
        return std::stoul(PQgetvalue(result_.get(), row, col));
    }
};

using ConnectionPtr = std::unique_ptr<Connection>;
} // namespace db::connection