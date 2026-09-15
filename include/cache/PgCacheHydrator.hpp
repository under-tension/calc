#pragma once

#include "cache/ICache.hpp"
#include "db/repository/OperationRepository.hpp"
#include "model/OperationModel.hpp"

#include <vector>

namespace cache
{
class PgCacheHydrator
{
  private:
    db::repository::OperationRepository& operationRepository_;
    cache::ICache& cache_;

  public:
    PgCacheHydrator(db::repository::OperationRepository& repo,
                    cache::ICache& cache) :
        operationRepository_(repo), cache_(cache)
    {}

    void hydrate();
};
} // namespace cache