//
// Created by denis on 16.07.2022.
//

#ifndef COURSEWORKDB_IREPOSITORYFACTORY_H
#define COURSEWORKDB_IREPOSITORYFACTORY_H

#include "UserRepository.h"
#include "TournamentRepository.h"
#include "MatchRepository.h"
#include "TournamentParticipantsRepository.h"
#include "roles/IRole.h"

namespace polytour::db::repository {
    class IRepositoryFactory {
    public:

        virtual ~IRepositoryFactory() = default;

        virtual IRepository<transport::User>* getUserRepository() = 0;

        virtual IRepository<transport::Tournament>* getTournamentRepository() = 0;

        virtual IRepository<transport::Match>* getMatchRepository() = 0;

        virtual IRepository<transport::TournamentParticipant>* getTournamentParticipantsRepository() = 0;

    protected:

        static transport::User roleToUser(const std::shared_ptr<roles::IRole>& role) {
            return role->toUser();
        }

    };
}

#endif //COURSEWORKDB_IREPOSITORYFACTORY_H
