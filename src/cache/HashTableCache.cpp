#include "cache/HashTableCache.hpp"

namespace cache
{
bool OperationEq::operator()(const model::OperationModel& a,
                             const model::OperationModel& b) const noexcept
{
    if (a.operation_type != b.operation_type)
    {
        return false;
    }

    if (a.operation_type == "+" || a.operation_type == "*")
    {
        return (a.operand1 == b.operand1 && a.operand2 == b.operand2) ||
               (a.operand1 == b.operand2 && a.operand2 == b.operand1);
    }

    return a.operand1 == b.operand1 && a.operand2 == b.operand2;
};

size_t
    OperationHash::operator()(const model::OperationModel& model) const noexcept
{
    std::string hash = "";

    if (model.operation_type == "+" || model.operation_type == "*")
    {
        hash = model.operation_type +
               std::to_string(std::min(model.operand1, model.operand2)) +
               std::to_string(std::max(model.operand1, model.operand2));
    }
    else
    {
        hash = model.operation_type + std::to_string(model.operand1) +
               std::to_string(model.operand2);
    }

    return std::hash<std::string>{}(hash);
};

std::optional<model::OperationModel>
    HashTableCache::get(const model::OperationModel& key)
{
    auto it = map_.find(key);

    if (it != map_.end())
    {
        return it->second;
    }

    return std::nullopt;
}

void HashTableCache::set(const model::OperationModel& key,
                         const model::OperationModel& operationModel)
{
    map_[key] = operationModel;
}
} // namespace cache
