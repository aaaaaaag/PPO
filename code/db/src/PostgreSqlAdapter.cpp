//
// Created by denis on 10.06.2022.
//

#include "PostgreSqlAdapter.h"
#include "IDatabaseExecResult.h"
#include "private/pqxx/converters/ResultConverter.h"
#include "pqxx/pqxx"
#include "CriticalError.h"
#include "utility/JsonConfiguration.h"

class PostgreSqlExecResult: public polytour::db::IDatabaseExecResult {
public:

    explicit PostgreSqlExecResult(const pqxx::result& result):
            _fieldSet(polytour::db::priv::libpqxx::ResultConverter::toFieldSet(result))
            {};

    std::vector<polytour::db::utility::FieldSet> toFieldSetCollection() override {
        return _fieldSet;
    }

private:
    std::vector<polytour::db::utility::FieldSet> _fieldSet;
};

class polytour::db::PostgreSqlAdapter::PostgreSqlAdapterImpl {
public:

    explicit PostgreSqlAdapterImpl(const polytour::db::utility::FieldSet& connInfo):
        _conn(pqxx::connection(
                "user=" + connInfo["user"].toString() + " "
                "host=localhost "
                "password=" + connInfo["password"].toString() + " "
                "dbname=" + utility::JsonConfiguration::getInstance()->db_name
        )),
        _transaction(_conn)
        {
            transport::Logger::debug("Init postgresql adapter");
            _transaction.exec("begin;");
            _transaction.exec("savepoint f_savepoint;");
        }

    std::shared_ptr<IDatabaseExecResult> executeSqlCommand(const std::string& command) {
        try {
            transport::Logger::debug("Exec db command: " + command);
            return std::make_shared<PostgreSqlExecResult>(_transaction.exec(command));
        }
        catch (std::exception& ex) {
            _transaction.exec("rollback to savepoint f_savepoint;");
            throw polytour::CriticalError("DB command execution error: " + std::string(ex.what()));
        }
    }

    void commit() {
        transport::Logger::debug("commit postgresql transaction");
        _transaction.commit();
    }

private:

    pqxx::connection _conn{};
    pqxx::work _transaction;

};

polytour::db::PostgreSqlAdapter::PostgreSqlAdapter(const polytour::db::utility::FieldSet& connInfo):
_pImpl(std::make_unique<PostgreSqlAdapterImpl>(connInfo))
{}

std::shared_ptr<polytour::db::IDatabaseExecResult>
polytour::db::PostgreSqlAdapter::executeSqlCommand(const std::string& command) {
    return _pImpl->executeSqlCommand(command);
}

void polytour::db::PostgreSqlAdapter::commit() {
    _pImpl->commit();
}

polytour::db::PostgreSqlAdapter::~PostgreSqlAdapter() = default;


