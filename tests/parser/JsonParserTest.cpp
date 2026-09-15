#include "parser/JsonParser.hpp"

#include "model/OperationModel.hpp"

#include <gtest/gtest.h>

using model::OperationModel;
using model::StatusOperation;

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
    std::vector<std::pair<std::string, OperationModel>> cases = {
        {R"({"val1": 5, "val2": 3, "operation": "+"})",
         {"+", 5, 3, 8, static_cast<int>(StatusOperation::OK)}},
        {R"({"val1": 5, "val2": 3, "operation": "-"})",
         {"-", 5, 3, 2, static_cast<int>(StatusOperation::OK)}},
        {R"({"val1": 5, "val2": 3, "operation": "*"})",
         {"*", 5, 3, 15, static_cast<int>(StatusOperation::OK)}},
        {R"({"val1": 6, "val2": 3, "operation": "/"})",
         {"/", 6, 3, 2, static_cast<int>(StatusOperation::OK)}},
        {R"({"val1": 5, "operation": "!"})",
         {"!", 5, 0, 120, static_cast<int>(StatusOperation::OK)}}};

    for (const auto& [json, expectedOperationModel] : cases)
    {
        OperationModel operationModel{};
        EXPECT_NO_THROW(jsonParser.parse(json, operationModel));
        EXPECT_EQ(operationModel.operand1, expectedOperationModel.operand1);
        EXPECT_EQ(operationModel.operand2, expectedOperationModel.operand2);
        EXPECT_EQ(operationModel.operation_type,
                  expectedOperationModel.operation_type);
    }
}

TEST_F(JsonParserTest, ThrowsOnEmptyOperation)
{
    OperationModel operationModel{};
    EXPECT_THROW(jsonParser.parse(R"({"val1": 5, "val2": 3})", operationModel),
                 std::runtime_error);
}

TEST_F(JsonParserTest, ThrowsOnValuesNull)
{
    std::vector<std::string> cases = {R"({"val1": 5, "operation": "+"})",
                                      R"({"operation": "!"})"};

    for (const auto& json : cases)
    {
        OperationModel operationModel{};
        EXPECT_THROW(jsonParser.parse(json, operationModel),
                     std::runtime_error);
    }
}