#pragma once

#include "db/connection/Connection.hpp"
#include "model/OperationModel.hpp"

#include <libpq-fe.h>

#include <string>
#include <vector>

namespace db::repository
{
class OperationRepository
{
  private:
    db::connection::Connection& conn_;

  public:
    OperationRepository(db::connection::Connection& conn) : conn_(conn)
    {}

    void insert(const model::OperationModel& operation);
    std::vector<model::OperationModel> findAll();
};
} // namespace db::repository