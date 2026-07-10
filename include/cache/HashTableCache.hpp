#pragma once

#include "cache/ICache.hpp"
#include "model/OperationModel.hpp"

#include <functional>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <vector>

namespace cache
{
struct OperationEq
{
    bool operator()(const model::OperationModel& a,
                    const model::OperationModel& b) const noexcept;
};

struct OperationHash
{
    size_t operator()(const model::OperationModel& t) const noexcept;
};

class HashTableCache : public ICache
{
  private:
    std::unordered_map<model::OperationModel, model::OperationModel,
                       OperationHash, OperationEq>
        map_;

  public:
    HashTableCache() = default;
    HashTableCache(const HashTableCache&) = delete;
    HashTableCache& operator=(const HashTableCache&) = delete;

    virtual std::optional<model::OperationModel>
        get(const model::OperationModel& key) override;
    virtual void set(const model::OperationModel& key,
                     const model::OperationModel& operationModel) override;
};
} // namespace cache
