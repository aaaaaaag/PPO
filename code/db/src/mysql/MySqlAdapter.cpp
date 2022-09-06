//
// Created by denis on 28.08.2022.
//

#include "public/mysql/MySqlAdapter.h"
#include "IDatabaseExecResult.h"
#include "private/cppconn/converters/ResultConverter.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include "CriticalError.h"
#include "utility/JsonConfiguration.h"

class MySqlExecResult: public polytour::db::IDatabaseExecResult {
public:

    explicit MySqlExecResult(sql::ResultSet* result):
            _fieldSet(polytour::db::priv::cppconn::ResultConverter::toFieldSet(result))
    {};

    std::vector<polytour::db::utility::FieldSet> toFieldSetCollection() override {
        return _fieldSet;
    }

private:
    std::vector<polytour::db::utility::FieldSet> _fieldSet;
};

class polytour::db::MySqlAdapter::MySqlAdapterImpl {
public:

    explicit MySqlAdapterImpl(const polytour::db::utility::FieldSet& connInfo) {
        try {
            driver = get_driver_instance();
            con = driver->connect("localhost", connInfo["user"].toString(), connInfo["password"].toString());
            con->setSchema(utility::JsonConfiguration::getInstance()->db_name);
            transport::Logger::debug("Init Mysql adapter");
        } catch (std::exception& ex) {
            throw polytour::CriticalError("Init conn error: " + std::string(ex.what()));
        }
    }

    std::shared_ptr<IDatabaseExecResult> executeSqlCommand(const std::string& command) {
        try {
            transport::Logger::debug("Exec db command: " + command);
            stmt = con->createStatement();
            if (command.find("select") != std::string::npos)
                return std::make_shared<MySqlExecResult>(stmt->executeQuery(command));
            else {
                stmt->execute(command);
                return nullptr;
            }
        }
        catch (sql::SQLException &ex) {
            auto errCode = ex.getErrorCode();
            transport::Logger::error("ErrCode: " + std::to_string(errCode));
            transport::Logger::error(ex.getSQLState());
            auto what = ex.what();
            throw polytour::CriticalError("DB command execution error: " + std::string(ex.what()));
        }
        catch (std::exception& ex) {
            auto what = ex.what();
            throw polytour::CriticalError("DB command execution error: " + std::string(ex.what()));
        }
    }

    void commit() {
        transport::Logger::debug("commit Mysql transaction");
    }

    ~MySqlAdapterImpl() {
        delete con;
        delete stmt;
    }

private:

    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt{};

};

polytour::db::MySqlAdapter::MySqlAdapter(const polytour::db::utility::FieldSet& connInfo):
        _pImpl(std::make_unique<MySqlAdapterImpl>(connInfo))
{}

std::shared_ptr<polytour::db::IDatabaseExecResult>
polytour::db::MySqlAdapter::executeSqlCommand(const std::string& command) {
    return _pImpl->executeSqlCommand(command);
}

void polytour::db::MySqlAdapter::commit() {
    _pImpl->commit();
}

polytour::db::MySqlAdapter::~MySqlAdapter() = default;



