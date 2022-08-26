//
// Created by denis on 25.08.2022.
//

#ifndef COURSEWORKDB_MOCKREPOSITORYFACTORY_H
#define COURSEWORKDB_MOCKREPOSITORYFACTORY_H

#include "gmock/gmock.h"
#include "repositories/IRepositoryFactory.h"

namespace polytour::db::tests::mocks {
    class MockRepositoryFactory: public repository::IRepositoryFactory {
    public:
        MOCK_METHOD(repository::IRepository<transport::User>*, getUserRepository, (), (override));
        MOCK_METHOD(repository::IRepository<transport::Tournament>*, getTournamentRepository, (), (override));
        MOCK_METHOD(repository::IRepository<transport::Match>*, getMatchRepository, (), (override));
        MOCK_METHOD(repository::IRepository<transport::TournamentParticipant>*, getTournamentParticipantsRepository, (), (override));
    };
}

#endif //COURSEWORKDB_MOCKREPOSITORYFACTORY_H
