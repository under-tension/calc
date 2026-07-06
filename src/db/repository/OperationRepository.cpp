#include "db/repository/OperationRepository.hpp"

namespace db::repository
{
void OperationRepository::insert(const model::OperationModel& operation)
{
    const std::string query =
        "INSERT INTO operations (operation_type, operand1, operand2, result, status) VALUES ($1, $2, $3, $4, $5)";

    const std::string typeStr = operation.operation_type;
    const std::string op1Str = std::to_string(operation.operand1);
    const std::string op2Str = std::to_string(operation.operand2);
    const std::string resultStr = std::to_string(operation.result);
    const std::string statusStr = std::to_string(operation.status);

    const char* paramValues[5] = {typeStr.c_str(), op1Str.c_str(),
                                  op2Str.c_str(), resultStr.c_str(),
                                  statusStr.c_str()};

    PGresult* res = PQexecParams(conn_.get(), query.c_str(), 5, nullptr,
                                 paramValues, nullptr, nullptr, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        std::string err_msg = PQerrorMessage(conn_.get());
        PQclear(res);
        throw std::runtime_error("Insert operation failed: " + err_msg);
    }

    PQclear(res);
}

std::vector<model::OperationModel> OperationRepository::findAll()
{
    std::vector<model::OperationModel> result;

    const std::string query =
        "SELECT id, operation_type, operand1, operand2, result, status FROM operations";

    PGresult* raw_res = PQexecParams(conn_.get(), query.c_str(), 0, nullptr,
                                     nullptr, nullptr, nullptr, 0);

    if (PQresultStatus(raw_res) != PGRES_TUPLES_OK)
    {
        std::string err_msg = PQerrorMessage(conn_.get());
        PQclear(raw_res);
        throw std::runtime_error("Select operation failed: " + err_msg);
    }

    db::connection::QueryResultPtr res =
        std::make_unique<db::connection::QueryResult>(raw_res);

    for (unsigned i = 0; i < res->rowCount(); ++i)
    {
        result.push_back(model::OperationModel(
            res->getUInt(i, 0), res->getString(i, 1), res->getInt(i, 2),
            res->getInt(i, 3), res->getInt(i, 4), res->getInt(i, 5)));
    }

    return result;
}
} // namespace db::repository