#include "Calculator.hpp"

#include "model/OperationModel.hpp"

#include <gtest/gtest.h>

using model::OperationModel;
using model::StatusOperation;

class CalculatorTest : public testing::Test
{
  protected:
    app::Calculator calculator;

    CalculatorTest()
    {
        calculator = app::Calculator();
    };

    ~CalculatorTest() override = default;
};

TEST_F(CalculatorTest, HandlesPositiveInput)
{
    std::vector<OperationModel> operations = {
        {"+", 5, 3, 8, static_cast<int>(StatusOperation::OK)},
        {"-", 5, 3, 2, static_cast<int>(StatusOperation::OK)},
        {"*", 5, 3, 15, static_cast<int>(StatusOperation::OK)},
        {"/", 6, 3, 2, static_cast<int>(StatusOperation::OK)},
        {"!", 5, 0, 120, static_cast<int>(StatusOperation::OK)}};

    for (auto& operationModel : operations)
    {
        OperationModel operation = operationModel;
        calculator.calculate(operation);
        EXPECT_EQ(operation.result, operationModel.result);
        EXPECT_EQ(operation.status, static_cast<int>(operationModel.status));
    }
}

TEST_F(CalculatorTest, DivisionByZero)
{
    OperationModel operationModel = {
        "/", 6, 0, 0, static_cast<int>(StatusOperation::DIVISION_BY_ZERO)};
    calculator.calculate(operationModel);
    EXPECT_EQ(operationModel.status,
              static_cast<int>(StatusOperation::DIVISION_BY_ZERO));
}

TEST_F(CalculatorTest, NegativeDegree)
{
    OperationModel operationModel = {
        "^", 2, -3, 0, static_cast<int>(StatusOperation::NEGATIVE_DEGREE)};
    calculator.calculate(operationModel);
    EXPECT_EQ(operationModel.status,
              static_cast<int>(StatusOperation::NEGATIVE_DEGREE));
}

TEST_F(CalculatorTest, NegativeFactorial)
{
    OperationModel operationModel = {
        "!", -5, 0, 0, static_cast<int>(StatusOperation::NEGATIVE_FACTORIAL)};
    calculator.calculate(operationModel);
    EXPECT_EQ(operationModel.status,
              static_cast<int>(StatusOperation::NEGATIVE_FACTORIAL));
}

TEST_F(CalculatorTest, NoOperation)
{
    OperationModel operationModel = {
        "=", 5, 3, 0, static_cast<int>(StatusOperation::NO_OPERATION)};
    calculator.calculate(operationModel);
    EXPECT_EQ(operationModel.status,
              static_cast<int>(StatusOperation::NO_OPERATION));
}
