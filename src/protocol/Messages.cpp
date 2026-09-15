#include "protocol/Messages.hpp"

#include <stdexcept>

namespace protocol
{
std::string CalculationRequest::describe() const
{
    if (operation == "!")
    {
        return std::to_string(operand1) + "!";
    }

    return std::to_string(operand1) + " " + operation + " " +
           std::to_string(operand2);
}

nlohmann::json CalculationRequest::toJson() const
{
    return nlohmann::json{{"type", "request"},
                          {"operation", operation},
                          {"val1", operand1},
                          {"val2", operand2}};
}

CalculationRequest CalculationRequest::fromJson(const nlohmann::json& json)
{
    if (json.contains("operation") == false)
    {
        throw std::runtime_error("Operation cannot be empty");
    }

    const std::string operation = json.at("operation").get<std::string>();
    if (operation.empty())
    {
        throw std::runtime_error("Operation cannot be empty");
    }

    CalculationRequest request;
    request.operation = operation.substr(0, 1);

    if (request.operation == "!")
    {
        if (json.contains("val1") == false)
        {
            throw std::runtime_error("Values cannot be null");
        }

        request.operand1 = json.at("val1").get<int>();

        return request;
    }

    if (json.contains("val1") == false || json.contains("val2") == false)
    {
        throw std::runtime_error("Values cannot be null");
    }

    request.operand1 = json.at("val1").get<int>();
    request.operand2 = json.at("val2").get<int>();

    return request;
}

nlohmann::json CalculationResponse::toJson() const
{
    return nlohmann::json{{"type", "response"},
                          {"result", result},
                          {"status", status},
                          {"error", error}};
}

CalculationResponse CalculationResponse::fromJson(const nlohmann::json& json)
{
    CalculationResponse response;

    response.result = json.value("result", 0);
    response.status = json.value("status", 0);
    response.error = json.value("error", std::string());

    return response;
}

nlohmann::json ShutdownNotice::toJson() const
{
    return nlohmann::json{{"type", "shutdown"}, {"message", message}};
}

ShutdownNotice ShutdownNotice::fromJson(const nlohmann::json& json)
{
    ShutdownNotice notice;

    notice.message = json.value("message", std::string("Server is shutting down"));

    return notice;
}

MessageType messageTypeOf(const nlohmann::json& json)
{
    if (json.contains("type") == false)
    {
        return MessageType::REQUEST;
    }

    const std::string type = json.at("type").get<std::string>();

    if (type == "request")
    {
        return MessageType::REQUEST;
    }

    if (type == "response")
    {
        return MessageType::RESPONSE;
    }

    if (type == "shutdown")
    {
        return MessageType::SHUTDOWN;
    }

    return MessageType::UNKNOWN;
}

std::string encodeLine(const nlohmann::json& json)
{
    return json.dump() + "\n";
}
} // namespace protocol