//
// Created by denis on 16.07.2022.
//

#ifndef COURSEWORKDB_REPOSITORYFACTORY_H
#define COURSEWORKDB_REPOSITORYFACTORY_H

#include "IRepositoryFactory.h"

namespace polytour::db::repository {
    class RepositoryFactory: public IRepositoryFactory{
    public:

        RepositoryFactory() = delete;

        explicit RepositoryFactory(const std::shared_ptr<roles::IRole>& role);

        ~RepositoryFactory() override;

        UserRepository* getUserRepository() override;

        TournamentRepository* getTournamentRepository() override;

        MatchRepository* getMatchRepository() override;

        TournamentParticipantsRepository *getTournamentParticipantsRepository() override;

    private:

        explicit RepositoryFactory(const transport::User& activeUser);

        std::shared_ptr<IConnection> _pConn;
        std::unique_ptr<UserRepository> _pUserRepo;
        std::unique_ptr<TournamentRepository> _pTournamentRepo;
        std::unique_ptr<MatchRepository> _pMatchRepo;
        std::unique_ptr<TournamentParticipantsRepository> _pTournamentParticipantsRepo;
    };
}


#endif //COURSEWORKDB_REPOSITORYFACTORY_H
