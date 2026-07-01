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

    PGresult* res = PQexecParams(conn_->get(), query.c_str(), 5, nullptr,
                                 paramValues, nullptr, nullptr, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        std::string err_msg = PQerrorMessage(conn_->get());
        PQclear(res);
        throw std::runtime_error("Insert operation failed: " + err_msg);
    }

    PQclear(res);
}

// std::optional<model::OperationModel> OperationRepository::findById(unsigned
// id)
// {
//     const std::string query = "SELECT id, operation_type, operand1, operand2,
//     result, status FROM operations WHERE id = $1";

//     const std::string idStr = std::to_string(id);
//     const char* paramValues[1] = { idStr.c_str() };

//     PGresult* res = PQexecParams(
//         conn_->get(),
//         query.c_str(),
//         1,
//         nullptr,
//         paramValues,
//         nullptr,
//         nullptr,
//         0
//     );

//     if (PQresultStatus(res) != PGRES_TUPLES_OK)
//     {
//         std::string err_msg = PQerrorMessage(conn_->get());
//         PQclear(res);
//         throw std::runtime_error("Select operation failed: " + err_msg);
//     }

//     int rowCount = PQntuples(res);
//     if (rowCount == 0)
//     {
//         PQclear(res);
//         return std::nullopt;
//     }

//     model::OperationModel operation(
//         std::stoul(PQgetvalue(res, 0, 0)),
//         PQgetvalue(res, 0, 1),
//         std::stoi(PQgetvalue(res, 0, 2)),
//         std::stoi(PQgetvalue(res, 0, 3)),
//         std::stoi(PQgetvalue(res, 0, 4)),
//         std::stoi(PQgetvalue(res, 0, 5))
//     );

//     PQclear(res);

//     return operation;
// }
} // namespace db::repository