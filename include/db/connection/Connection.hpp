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
    PGconn* conn_;

  public:
    Connection(const std::string& conninfo)
    {
        conn_ = PQconnectdb(conninfo.c_str());

        if (PQstatus(conn_) != CONNECTION_OK)
        {
            std::string err_msg = PQerrorMessage(conn_);
            conn_ = nullptr;
            throw std::runtime_error("Connection to database failed: " +
                                     err_msg);
        }
    }

    ~Connection()
    {
        if (conn_)
        {
            PQfinish(conn_);
        }
    }

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    Connection(Connection&& other) noexcept
    {
        conn_ = other.conn_;
        other.conn_ = nullptr;
    }

    Connection& operator=(Connection&& other) noexcept
    {
        if (this != &other)
        {
            PQfinish(conn_);
            conn_ = other.conn_;
            other.conn_ = nullptr;
        }

        return *this;
    }

    PGconn* get() const
    {
        return conn_;
    }

    PGconn* operator->() const
    {
        return conn_;
    }
};

class QueryResult
{
  private:
    PGresult* result_;

  public:
    explicit QueryResult(PGresult* res) : result_(res)
    {
        if (!result_)
            throw std::runtime_error("Null PGresult provided");
    }

    ~QueryResult()
    {
        if (result_)
        {
            PQclear(result_);
        }
    }

    QueryResult(const QueryResult&) = delete;
    QueryResult& operator=(const QueryResult&) = delete;

    QueryResult(QueryResult&& other) noexcept
    {
        result_ = other.result_;
        other.result_ = nullptr;
    }

    QueryResult& operator=(QueryResult&& other) noexcept
    {
        if (this != &other)
        {
            if (result_)
            {
                PQclear(result_);
            }

            result_ = other.result_;
            other.result_ = nullptr;
        }

        return *this;
    }

    PGresult* get() const
    {
        return result_;
    }

    PGresult* operator->() const
    {
        return result_;
    }

    int rowCount() const
    {
        return PQntuples(result_);
    }

    int columnCount() const
    {
        return PQnfields(result_);
    }

    bool isNull(int row, int col) const
    {
        return PQgetisnull(result_, row, col);
    }

    std::string getString(int row, int col) const
    {
        if (isNull(row, col))
        {
            return "";
        }

        return PQgetvalue(result_, row, col);
    }

    int getInt(int row, int col) const
    {
        if (isNull(row, col))
        {
            throw std::runtime_error("Cannot get int from NULL column");
        }

        return std::stoi(PQgetvalue(result_, row, col));
    }

    unsigned getUInt(int row, int col) const
    {
        if (isNull(row, col))
            throw std::runtime_error("NULL column");
        return std::stoul(PQgetvalue(result_, row, col));
    }
};

using ConnectionPtr = std::unique_ptr<Connection>;
using QueryResultPtr = std::unique_ptr<QueryResult>;
} // namespace db::connection