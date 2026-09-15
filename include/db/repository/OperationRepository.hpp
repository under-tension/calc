#pragma once

#include "db/connection/Connection.hpp"
#include "model/OperationModel.hpp"

#include <postgresql/libpq-fe.h>

#include <memory>
#include <optional>
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
    // std::optional<model::OperationModel> findById(unsigned id);
};
} // namespace db::repository