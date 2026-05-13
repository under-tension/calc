#include "Calculator.hpp"

#include "Task.hpp"

#include <gtest/gtest.h>

using app::StatusTask;
using app::Task;

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
    std::vector<Task> tasks = {
        {5, 3, 8, static_cast<int>(StatusTask::OK), '+'},
        {5, 3, 2, static_cast<int>(StatusTask::OK), '-'},
        {5, 3, 15, static_cast<int>(StatusTask::OK), '*'},
        {6, 3, 2, static_cast<int>(StatusTask::OK), '/'},
        {5, 0, 120, static_cast<int>(StatusTask::OK), '!'}};

    for (auto& task : tasks)
    {
        Task t = task;
        calculator.calculate(t);
        EXPECT_EQ(t.result, task.result);
        EXPECT_EQ(t.status, static_cast<int>(task.status));
    }
}

TEST_F(CalculatorTest, DivisionByZero)
{
    Task task = {6, 0, 0, static_cast<int>(StatusTask::DIVISION_BY_ZERO), '/'};
    calculator.calculate(task);
    EXPECT_EQ(task.status, static_cast<int>(StatusTask::DIVISION_BY_ZERO));
}

TEST_F(CalculatorTest, NegativeDegree)
{
    Task task = {2, -3, 0, static_cast<int>(StatusTask::NEGATIVE_DEGREE), '^'};
    calculator.calculate(task);
    EXPECT_EQ(task.status, static_cast<int>(StatusTask::NEGATIVE_DEGREE));
}

TEST_F(CalculatorTest, NegativeFactorial)
{
    Task task = {-5, 0, 0, static_cast<int>(StatusTask::NEGATIVE_FACTORIAL),
                 '!'};
    calculator.calculate(task);
    EXPECT_EQ(task.status, static_cast<int>(StatusTask::NEGATIVE_FACTORIAL));
}

TEST_F(CalculatorTest, NoOperation)
{
    Task task = {5, 3, 0, static_cast<int>(StatusTask::NO_OPERATION), '='};
    calculator.calculate(task);
    EXPECT_EQ(task.status, static_cast<int>(StatusTask::NO_OPERATION));
}
