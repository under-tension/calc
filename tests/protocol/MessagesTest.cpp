#include "protocol/Messages.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

using protocol::CalculationRequest;
using protocol::CalculationResponse;
using protocol::MessageType;
using protocol::ShutdownNotice;

TEST(CalculationRequestTest, SurvivesRoundTrip)
{
    const CalculationRequest source("+", 2, 4);

    const CalculationRequest restored =
        CalculationRequest::fromJson(source.toJson());

    EXPECT_EQ(restored.operation, source.operation);
    EXPECT_EQ(restored.operand1, source.operand1);
    EXPECT_EQ(restored.operand2, source.operand2);
}

TEST(CalculationRequestTest, TreatsMessageWithoutTypeAsRequest)
{
    const nlohmann::json json =
        nlohmann::json::parse(R"({"val1": 2, "val2": 4, "operation": "+"})");

    EXPECT_EQ(protocol::messageTypeOf(json), MessageType::REQUEST);
    EXPECT_EQ(CalculationRequest::fromJson(json).operand2, 4);
}

TEST(CalculationRequestTest, RejectsIncompleteMessages)
{
    const std::vector<std::string> cases = {
        R"({"val1": 5, "val2": 3})",       // операции нет
        R"({"val1": 5, "operation": ""})", // операция пустая
        R"({"val1": 5, "operation": "+"})" // второго операнда нет
    };

    for (const std::string& json : cases)
    {
        EXPECT_THROW(CalculationRequest::fromJson(nlohmann::json::parse(json)),
                     std::runtime_error)
            << json;
    }
}

TEST(CalculationRequestTest, NeedsOnlyFirstOperandForFactorial)
{
    const CalculationRequest request = CalculationRequest::fromJson(
        nlohmann::json::parse(R"({"val1": 5, "operation": "!"})"));

    EXPECT_EQ(request.operation, "!");
    EXPECT_EQ(request.operand1, 5);
}

TEST(CalculationRequestTest, DescribesTaskForLogs)
{
    EXPECT_EQ(CalculationRequest("/", 5, 0).describe(), "5 / 0");
    EXPECT_EQ(CalculationRequest("!", 3, 0).describe(), "3!");
}

TEST(CalculationResponseTest, SurvivesRoundTrip)
{
    const CalculationResponse source(6, 2, "Error! Division by zero");

    const CalculationResponse restored =
        CalculationResponse::fromJson(source.toJson());

    EXPECT_EQ(restored.result, source.result);
    EXPECT_EQ(restored.status, source.status);
    EXPECT_EQ(restored.error, source.error);
}

TEST(CalculationResponseTest, ReportsFailureByStatus)
{
    EXPECT_FALSE(CalculationResponse(6, 0, "").failed());
    EXPECT_TRUE(CalculationResponse(0, 2, "division by zero").failed());
    // Текст может потеряться, статус — нет.
    EXPECT_TRUE(CalculationResponse(0, 2, "").failed());
}

TEST(MessageTypeTest, RecognizesEveryKind)
{
    EXPECT_EQ(protocol::messageTypeOf(CalculationRequest().toJson()),
              MessageType::REQUEST);
    EXPECT_EQ(protocol::messageTypeOf(CalculationResponse().toJson()),
              MessageType::RESPONSE);
    EXPECT_EQ(protocol::messageTypeOf(ShutdownNotice().toJson()),
              MessageType::SHUTDOWN);
    EXPECT_EQ(protocol::messageTypeOf(nlohmann::json{{"type", "nonsense"}}),
              MessageType::UNKNOWN);
}

TEST(EncodeLineTest, PutsOneMessageOnOneLine)
{
    const std::string line = protocol::encodeLine(CalculationRequest("+", 1, 2).toJson());

    ASSERT_FALSE(line.empty());
    EXPECT_EQ(line.back(), '\n');
    // Разделитель встречается ровно один раз, иначе приёмник увидит два
    // сообщения вместо одного.
    EXPECT_EQ(line.find('\n'), line.size() - 1);
}
