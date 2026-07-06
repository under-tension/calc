#include "cache/PgCacheHydrator.hpp"

namespace cache
{
void PgCacheHydrator::hydrate()
{
    std::vector<model::OperationModel> models = operationRepository_.findAll();

    for (const model::OperationModel& element : models)
    {
        cache_.set(element, element);
    }
}
} // namespace cache