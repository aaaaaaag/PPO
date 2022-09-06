//
// Created by denis on 7/12/22.
//

#include "SqlConnection.h"

using namespace polytour::db;

void polytour::db::SqlConnection::addObj(const polytour::db::utility::FieldSet &fieldSet,
                                         const polytour::db::repository::Identity &identity) {
    auto strCommand = _pCommandSource->addObj(fieldSet, identity);
    _pDbAdapter->executeSqlCommand(strCommand);
}

void polytour::db::SqlConnection::deleteObj(const polytour::db::utility::FieldSet &fieldSet,
                                            const polytour::db::repository::Identity &identity) {
    auto strCommand = _pCommandSource->deleteObj(fieldSet, identity);
    _pDbAdapter->executeSqlCommand(strCommand);
}

void polytour::db::SqlConnection::updateObj(const utility::FieldSet& curFieldSet,
                                            const utility::FieldSet& newFieldSet,
                                            const repository::Identity& identity) {
    auto strCommand = _pCommandSource->updateObj(curFieldSet, newFieldSet, identity);
    _pDbAdapter->executeSqlCommand(strCommand);
}

std::vector<utility::FieldSet> polytour::db::SqlConnection::searchObj(const polytour::db::utility::FieldSet &fieldSet,
                                                                      const polytour::db::repository::Identity &identity) {
    auto strCommand = _pCommandSource->searchObj(fieldSet, identity);
    auto dbResult = _pDbAdapter->executeSqlCommand(strCommand);
    return dbResult->toFieldSetCollection();
}

void SqlConnection::init(const repository::Identity &identity) {
    auto strCommand = _pCommandSource->initTable(identity);
    _pDbAdapter->executeSqlCommand(strCommand);
}

polytour::db::SqlConnection::SqlConnection(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                                           std::shared_ptr<utility::ICommandSource> commandSource):
    _pDbAdapter(std::move(dbAdapter)),
    _pCommandSource(std::move(commandSource))
    {}

void SqlConnection::commit() {
    _pDbAdapter->commit();
}
