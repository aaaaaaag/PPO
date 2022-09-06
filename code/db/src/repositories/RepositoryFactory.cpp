//
// Created by denis on 16.07.2022.
//

#include "repositories/RepositoryFactory.h"
#include "SqlConnection.h"
#include "PostgreSqlAdapter.h"
#include "utility/PostgreSqlCommandSource.h"
#include "utility/JsonConfiguration.h"

using namespace polytour::db::repository;

polytour::db::repository::RepositoryFactory::RepositoryFactory(const transport::User& activeUser) {
    utility::FieldSet connInfo;
    connInfo.addPair("user", utility::TableAbstractValue(activeUser.nickname));
    connInfo.addPair("password", utility::TableAbstractValue(activeUser.password));

    auto sqlType = utility::JsonConfiguration::getInstance()->used_sql;
    switch (sqlType) {
        case utility::DbConfiguration::Sqls::Postgres:
            _pConn = std::make_shared<SqlConnection>(std::make_shared<PostgreSqlAdapter>(connInfo),
                                                     std::make_shared<utility::PostgreSqlCommandSource>());
            break;
        case utility::DbConfiguration::Sqls::MongoDb:
        default:
            throw std::logic_error("WOW we dont have mongodb");
    }


    _pUserRepo = std::make_unique<decltype(_pUserRepo)::element_type>(_pConn);
    _pTournamentRepo = std::make_unique<decltype(_pTournamentRepo)::element_type>(_pConn);
    _pMatchRepo = std::make_unique<decltype(_pMatchRepo)::element_type>(_pConn);
    _pTournamentParticipantsRepo = std::make_unique<decltype(_pTournamentParticipantsRepo)::element_type>(_pConn);

    _pUserRepo->init();
    _pTournamentRepo->init();
    _pTournamentParticipantsRepo->init();
    _pMatchRepo->init();
}

UserRepository* polytour::db::repository::RepositoryFactory::getUserRepository() {
    return _pUserRepo.get();
}

TournamentRepository* polytour::db::repository::RepositoryFactory::getTournamentRepository() {
    return _pTournamentRepo.get();
}

MatchRepository *RepositoryFactory::getMatchRepository() {
    return _pMatchRepo.get();
}

TournamentParticipantsRepository *RepositoryFactory::getTournamentParticipantsRepository() {
    return _pTournamentParticipantsRepo.get();
}

RepositoryFactory::~RepositoryFactory() {
    _pConn->commit();
}

RepositoryFactory::RepositoryFactory(const std::shared_ptr<roles::IRole> &role):
        RepositoryFactory(roleToUser(role)){}

