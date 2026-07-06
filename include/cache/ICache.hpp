#pragma once

#include "model/OperationModel.hpp"

#include <functional>
#include <optional>
#include <string>

namespace cache
{
class ICache
{
  public:
  virtual ~ICache() = default;
    virtual std::optional<model::OperationModel>
        get(const model::OperationModel& key) = 0;
    virtual void set(const model::OperationModel& key,
                     const model::OperationModel& operationModel) = 0;
};
} // namespace cache