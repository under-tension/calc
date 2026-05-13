#include "Checker.hpp"

#include "Task.hpp"

#include <gtest/gtest.h>

using app::StatusTask;
using app::Task;

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
    Task task = {5, 3, 8, static_cast<int>(StatusTask::OK), '+'};
    EXPECT_NO_THROW(checker.check(task));
}

TEST_F(CheckerTest, NotEnoughArgs)
{
    Task task = {5, 0, 0, static_cast<int>(StatusTask::NOT_ENOUGH_ARGS), '+'};
    EXPECT_THROW(checker.check(task), std::runtime_error);
}

TEST_F(CheckerTest, NoOperation)
{
    Task task = {5, 3, 0, static_cast<int>(StatusTask::NO_OPERATION), '='};
    EXPECT_THROW(checker.check(task), std::runtime_error);
}

TEST_F(CheckerTest, DivisionByZero)
{
    Task task = {5, 0, 0, static_cast<int>(StatusTask::DIVISION_BY_ZERO), '/'};
    EXPECT_THROW(checker.check(task), std::runtime_error);
}

TEST_F(CheckerTest, NegativeDegree)
{
    Task task = {2, -3, 0, static_cast<int>(StatusTask::NEGATIVE_DEGREE), '^'};
    EXPECT_THROW(checker.check(task), std::runtime_error);
}

TEST_F(CheckerTest, NegativeFactorial)
{
    Task task = {-5, 0, 0, static_cast<int>(StatusTask::NEGATIVE_FACTORIAL),
                 '!'};
    EXPECT_THROW(checker.check(task), std::runtime_error);
}

TEST_F(CheckerTest, EmptyOperation)
{
    Task task = {5, 3, 0, static_cast<int>(StatusTask::NO_OPERATION), '\0'};
    EXPECT_THROW(checker.check(task), std::runtime_error);
}

TEST_F(CheckerTest, UnknownError)
{
    Task task = {5, 3, 0, -3, '@'};
    EXPECT_THROW(checker.check(task), std::runtime_error);
}