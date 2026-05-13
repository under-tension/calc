#include "parser/JsonParser.hpp"

#include "Task.hpp"

#include <gtest/gtest.h>

using app::StatusTask;
using app::Task;

class JsonParserTest : public testing::Test
{
  protected:
    parsers::JsonParser jsonParser;

    JsonParserTest()
    {
        jsonParser = parsers::JsonParser();
    }

    ~JsonParserTest() override = default;
};

TEST_F(JsonParserTest, ParsesValidJson)
{
    std::vector<std::pair<std::string, Task>> cases = {
        {R"({"val1": 5, "val2": 3, "operation": "+"})",
         {5, 3, 8, static_cast<int>(StatusTask::OK), '+'}},
        {R"({"val1": 5, "val2": 3, "operation": "-"})",
         {5, 3, 2, static_cast<int>(StatusTask::OK), '-'}},
        {R"({"val1": 5, "val2": 3, "operation": "*"})",
         {5, 3, 15, static_cast<int>(StatusTask::OK), '*'}},
        {R"({"val1": 6, "val2": 3, "operation": "/"})",
         {6, 3, 2, static_cast<int>(StatusTask::OK), '/'}},
        {R"({"val1": 5, "operation": "!"})",
         {5, 0, 120, static_cast<int>(StatusTask::OK), '!'}}};

    for (const auto& [json, expectedTask] : cases)
    {
        Task task{};
        EXPECT_NO_THROW(jsonParser.parse(json, task));
        EXPECT_EQ(task.val1, expectedTask.val1);
        EXPECT_EQ(task.val2, expectedTask.val2);
        EXPECT_EQ(task.operation, expectedTask.operation);
    }
}

TEST_F(JsonParserTest, ThrowsOnEmptyOperation)
{
    Task task{};
    EXPECT_THROW(jsonParser.parse(R"({"val1": 5, "val2": 3})", task),
                 std::runtime_error);
}

TEST_F(JsonParserTest, ThrowsOnValuesNull)
{
    std::vector<std::string> cases = {R"({"val1": 5, "operation": "+"})",
                                      R"({"operation": "!"})"};

    for (const auto& json : cases)
    {
        Task task{};
        EXPECT_THROW(jsonParser.parse(json, task), std::runtime_error);
    }
}