//
// Created by denis on 25.08.2022.
//

#ifndef COURSEWORKDB_MOCKREPOSITORYFACTORYCREATOR_H
#define COURSEWORKDB_MOCKREPOSITORYFACTORYCREATOR_H

#include "repositories/IRepositoryFactoryCreator.h"
#include "gmock/gmock.h"

namespace polytour::db::tests::mocks {
    class MockRepositoryFactoryCreator: public repository::IRepositoryFactoryCreator {
    public:
        MOCK_METHOD(std::shared_ptr<repository::IRepositoryFactory>, create, (transport::Roles role), (override));
    };
}

#endif //COURSEWORKDB_MOCKREPOSITORYFACTORYCREATOR_H
