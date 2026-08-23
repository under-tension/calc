#include "net/Server.hpp"

#include "Calculator.hpp"
#include "Checker.hpp"
#include "protocol/Messages.hpp"

#include <boost/asio/connect.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>

#include <gtest/gtest.h>

#include <chrono>
#include <istream>
#include <optional>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace
{
constexpr auto RESPONSE_TIMEOUT = std::chrono::seconds(5);

// Расчёт без базы и кэша: тестам нужен только результат операции.
protocol::CalculationResponse calculate(
    const protocol::CalculationRequest& request)
{
    app::Calculator calculator;
    app::Checker checker;

    model::OperationModel operationModel;
    operationModel.operation_type = request.operation;
    operationModel.operand1 = request.operand1;
    operationModel.operand2 = request.operand2;

    calculator.calculate(operationModel);

    protocol::CalculationResponse response;
    response.result = operationModel.result;
    response.status = operationModel.status;

    try
    {
        checker.check(operationModel);
    }
    catch (const std::exception& e)
    {
        response.error = e.what();
    }

    return response;
}

// Тестовый клиент: отправляет строку и ждёт ответ, ограничивая ожидание по
// времени вместо пауз фиксированной длительности.
class TestClient
{
  private:
    boost::asio::io_context context_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf buffer_;

  public:
    explicit TestClient(unsigned short port) : socket_(context_)
    {
        boost::asio::ip::tcp::resolver resolver(context_);
        boost::asio::connect(socket_, resolver.resolve("127.0.0.1",
                                                       std::to_string(port)));
    }

    void send(const std::string& line)
    {
        boost::asio::write(socket_, boost::asio::buffer(line));
    }

    void send(const protocol::CalculationRequest& request)
    {
        send(protocol::encodeLine(request.toJson()));
    }

    // Возвращает следующее сообщение сервера или пустое значение, если ответа
    // не было в отведённое время либо соединение закрылось.
    std::optional<nlohmann::json> receive()
    {
        std::optional<nlohmann::json> message;

        boost::asio::async_read_until(
            socket_, buffer_, '\n',
            [this, &message](const boost::system::error_code& error,
                             std::size_t)
            {
                if (error)
                {
                    return;
                }

                std::istream stream(&buffer_);
                std::string line;
                std::getline(stream, line);

                message = nlohmann::json::parse(line);
            });

        context_.restart();
        context_.run_for(RESPONSE_TIMEOUT);

        return message;
    }
};

// Поднимает сервер на свободном порту в отдельном потоке.
class ServerFixture : public testing::Test
{
  protected:
    boost::asio::io_context context;
    net::Server server{context, "127.0.0.1", 0, calculate};
    std::optional<std::jthread> worker;

    void SetUp() override
    {
        server.start();
        worker.emplace([this] { context.run(); });
    }

    void TearDown() override
    {
        boost::asio::post(context, [this] { server.shutdown(); });
        context.stop();
        worker.reset();
    }

    unsigned short port()
    {
        return server.port();
    }
};
} // namespace

TEST_F(ServerFixture, ReturnsCalculationResult)
{
    TestClient client(port());

    client.send(protocol::CalculationRequest("+", 2, 4));

    const std::optional<nlohmann::json> message = client.receive();

    ASSERT_TRUE(message.has_value());

    const protocol::CalculationResponse response =
        protocol::CalculationResponse::fromJson(message.value());

    EXPECT_EQ(protocol::messageTypeOf(message.value()),
              protocol::MessageType::RESPONSE);
    EXPECT_EQ(response.result, 6);
    EXPECT_EQ(response.status, static_cast<int>(model::StatusOperation::OK));
    EXPECT_FALSE(response.failed());
}

TEST_F(ServerFixture, ServesSeveralRequestsOverOneConnection)
{
    TestClient client(port());

    const std::vector<std::pair<protocol::CalculationRequest, int>> cases = {
        {protocol::CalculationRequest("-", 9, 4), 5},
        {protocol::CalculationRequest("*", 3, 5), 15},
        {protocol::CalculationRequest("^", 2, 3), 8},
        {protocol::CalculationRequest("!", 4, 0), 24}};

    for (const auto& [request, expected] : cases)
    {
        client.send(request);

        const std::optional<nlohmann::json> message = client.receive();

        ASSERT_TRUE(message.has_value());
        EXPECT_EQ(protocol::CalculationResponse::fromJson(message.value())
                      .result,
                  expected);
    }
}

TEST_F(ServerFixture, ReportsCalculationError)
{
    TestClient client(port());

    client.send(protocol::CalculationRequest("/", 5, 0));

    const std::optional<nlohmann::json> message = client.receive();

    ASSERT_TRUE(message.has_value());

    const protocol::CalculationResponse response =
        protocol::CalculationResponse::fromJson(message.value());

    EXPECT_TRUE(response.failed());
    EXPECT_EQ(response.status,
              static_cast<int>(model::StatusOperation::DIVISION_BY_ZERO));
}

TEST_F(ServerFixture, ReportsMalformedRequestAndKeepsServing)
{
    TestClient client(port());

    client.send(std::string("not a json\n"));

    const std::optional<nlohmann::json> failure = client.receive();

    ASSERT_TRUE(failure.has_value());
    EXPECT_TRUE(
        protocol::CalculationResponse::fromJson(failure.value()).failed());

    // Соединение осталось рабочим.
    client.send(protocol::CalculationRequest("+", 1, 1));

    const std::optional<nlohmann::json> message = client.receive();

    ASSERT_TRUE(message.has_value());
    EXPECT_EQ(protocol::CalculationResponse::fromJson(message.value()).result,
              2);
}

TEST_F(ServerFixture, NotifiesClientOnShutdown)
{
    TestClient client(port());

    client.send(protocol::CalculationRequest("+", 2, 2));
    ASSERT_TRUE(client.receive().has_value());

    boost::asio::post(context, [this] { server.shutdown(); });

    const std::optional<nlohmann::json> message = client.receive();

    ASSERT_TRUE(message.has_value());
    EXPECT_EQ(protocol::messageTypeOf(message.value()),
              protocol::MessageType::SHUTDOWN);
}