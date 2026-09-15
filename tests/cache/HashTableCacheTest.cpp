#include "cache/HashTableCache.hpp"

#include <gtest/gtest.h>

using model::OperationModel;

class HashTableCacheTest : public testing::Test
{
  protected:
    cache::HashTableCache cache;

    // Ключом служит сама модель, поэтому в тестах она передаётся дважды.
    void store(const OperationModel& operationModel)
    {
        cache.set(operationModel, operationModel);
    }
};

TEST_F(HashTableCacheTest, ReturnsNothingWhenTaskIsUnknown)
{
    EXPECT_FALSE(cache.get(OperationModel("+", 2, 4, 0, 0)).has_value());
}

TEST_F(HashTableCacheTest, ReturnsStoredResult)
{
    store(OperationModel("+", 2, 4, 6, 0));

    const std::optional<OperationModel> found =
        cache.get(OperationModel("+", 2, 4, 0, 0));

    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found.value().result, 6);
}

TEST_F(HashTableCacheTest, TreatsSwappedOperandsAsSameTask)
{
    store(OperationModel("+", 2, 4, 6, 0));
    store(OperationModel("*", 3, 5, 15, 0));

    EXPECT_TRUE(cache.get(OperationModel("+", 4, 2, 0, 0)).has_value());
    EXPECT_TRUE(cache.get(OperationModel("*", 5, 3, 0, 0)).has_value());
}

TEST_F(HashTableCacheTest, KeepsOrderForNonCommutativeOperations)
{
    store(OperationModel("-", 9, 4, 5, 0));
    store(OperationModel("/", 8, 2, 4, 0));

    EXPECT_FALSE(cache.get(OperationModel("-", 4, 9, 0, 0)).has_value());
    EXPECT_FALSE(cache.get(OperationModel("/", 2, 8, 0, 0)).has_value());
}

TEST_F(HashTableCacheTest, DistinguishesOperationsOnSameOperands)
{
    store(OperationModel("+", 2, 4, 6, 0));

    EXPECT_FALSE(cache.get(OperationModel("-", 2, 4, 0, 0)).has_value());
    EXPECT_FALSE(cache.get(OperationModel("*", 2, 4, 0, 0)).has_value());
}

TEST_F(HashTableCacheTest, ReplacesResultOfRepeatedTask)
{
    store(OperationModel("+", 2, 4, 6, 0));
    store(OperationModel("+", 2, 4, 7, 0));

    const std::optional<OperationModel> found =
        cache.get(OperationModel("+", 2, 4, 0, 0));

    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found.value().result, 7);
}

TEST_F(HashTableCacheTest, KeepsFailedOperationStatus)
{
    store(OperationModel("/", 5, 0, 0,
                             static_cast<int>(model::StatusOperation::DIVISION_BY_ZERO)));

    const std::optional<OperationModel> found =
        cache.get(OperationModel("/", 5, 0, 0, 0));

    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found.value().status,
              static_cast<int>(model::StatusOperation::DIVISION_BY_ZERO));
}
