#include "Checker.hpp"

#include "model/OperationModel.hpp"

#include <gtest/gtest.h>

using model::OperationModel;
using model::StatusOperation;

class CheckerTest : public testing::Test
{
  protected:
    app::Checker checker;

    CheckerTest()
    {
        checker = app::Checker();
    }

    ~CheckerTest() override = default;
};

TEST_F(CheckerTest, HandlesPositiveInput)
{
    OperationModel operationModel = {"+", 5, 3, 8,
                                     static_cast<int>(StatusOperation::OK)};
    EXPECT_NO_THROW(checker.check(operationModel));
}

TEST_F(CheckerTest, NotEnoughArgs)
{
    OperationModel operationModel = {
        "+", 5, 0, 0, static_cast<int>(StatusOperation::NOT_ENOUGH_ARGS)};
    EXPECT_THROW(checker.check(operationModel), std::runtime_error);
}

TEST_F(CheckerTest, NoOperation)
{
    OperationModel operationModel = {
        "=", 5, 3, 0, static_cast<int>(StatusOperation::NO_OPERATION)};
    EXPECT_THROW(checker.check(operationModel), std::runtime_error);
}

TEST_F(CheckerTest, DivisionByZero)
{
    OperationModel operationModel = {
        "/", 5, 0, 0, static_cast<int>(StatusOperation::DIVISION_BY_ZERO)};
    EXPECT_THROW(checker.check(operationModel), std::runtime_error);
}

TEST_F(CheckerTest, NegativeDegree)
{
    OperationModel operationModel = {
        "^", 2, -3, 0, static_cast<int>(StatusOperation::NEGATIVE_DEGREE)};
    EXPECT_THROW(checker.check(operationModel), std::runtime_error);
}

TEST_F(CheckerTest, NegativeFactorial)
{
    OperationModel operationModel = {
        "!", -5, 0, 0, static_cast<int>(StatusOperation::NEGATIVE_FACTORIAL)};
    EXPECT_THROW(checker.check(operationModel), std::runtime_error);
}

TEST_F(CheckerTest, EmptyOperation)
{
    OperationModel operationModel = {
        "\0", 5, 3, 0, static_cast<int>(StatusOperation::NO_OPERATION)};
    EXPECT_THROW(checker.check(operationModel), std::runtime_error);
}

TEST_F(CheckerTest, UnknownError)
{
    OperationModel operationModel = {"@", 5, 3, 0, -3};
    EXPECT_THROW(checker.check(operationModel), std::runtime_error);
}