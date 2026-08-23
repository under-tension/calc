#pragma once

#include <nlohmann/json.hpp>

#include <string>

namespace protocol
{
// Статус ответа, когда сообщение не удалось разобрать или оно не является
// заданием на расчёт.
constexpr int PROTOCOL_ERROR_STATUS = -1;

enum class MessageType
{
    REQUEST,
    RESPONSE,
    SHUTDOWN,
    UNKNOWN
};

// Задание на расчёт: {"type": "request", "val1": 2, "val2": 4, "operation": "+"}
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

    // Читаемое описание задания для логов: "5 / 0", "3!".
    std::string describe() const;

    nlohmann::json toJson() const;
    static CalculationRequest fromJson(const nlohmann::json& json);
};

// Результат расчёта: {"type": "response", "result": 6, "status": 0, "error": ""}
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
        return !error.empty();
    }

    nlohmann::json toJson() const;
    static CalculationResponse fromJson(const nlohmann::json& json);
};

// Уведомление о завершении работы сервера:
// {"type": "shutdown", "message": "..."}
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

// Сообщение без поля type считается заданием на расчёт — так формат остаётся
// совместимым с заданиями, которые передаются в командной строке.
MessageType messageTypeOf(const nlohmann::json& json);

// Одно сообщение — одна строка JSON, завершённая переводом строки.
std::string encodeLine(const nlohmann::json& json);
} // namespace protocol