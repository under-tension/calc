#pragma once

#include <nlohmann/json.hpp>

#include <string>

namespace protocol
{
constexpr int PROTOCOL_ERROR_STATUS = -1;

enum class MessageType
{
    REQUEST,
    RESPONSE,
    SHUTDOWN,
    UNKNOWN
};

class CalculationRequest
{
  public:
    std::string operation = "";
    int operand1 = 0;
    int operand2 = 0;

    CalculationRequest() = default;

    CalculationRequest(const std::string& operation, int operand1,
                       int operand2) :
        operation(operation), operand1(operand1), operand2(operand2)
    {}

    std::string describe() const;

    nlohmann::json toJson() const;
    static CalculationRequest fromJson(const nlohmann::json& json);
};

class CalculationResponse
{
  public:
    int result = 0;
    int status = 0;
    std::string error = "";

    CalculationResponse() = default;

    CalculationResponse(int result, int status, const std::string& error) :
        result(result), status(status), error(error)
    {}

    bool failed() const
    {
        return status != 0;
    }

    nlohmann::json toJson() const;
    static CalculationResponse fromJson(const nlohmann::json& json);
};

class ShutdownNotice
{
  public:
    std::string message = "Server is shutting down";

    ShutdownNotice() = default;

    explicit ShutdownNotice(const std::string& message) : message(message)
    {}

    nlohmann::json toJson() const;
    static ShutdownNotice fromJson(const nlohmann::json& json);
};

MessageType messageTypeOf(const nlohmann::json& json);
std::string encodeLine(const nlohmann::json& json);
} // namespace protocol