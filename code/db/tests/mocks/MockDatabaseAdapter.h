//
// Created by denis on 7/16/22.
//

#ifndef COURSEWORKDB_MOCKDATABASEADAPTER_H
#define COURSEWORKDB_MOCKDATABASEADAPTER_H

#include "IDatabaseAdapter.h"
#include "gmock/gmock.h"

namespace polytour::db::tests::mocks {
    class MockDatabaseAdapter: public IDatabaseAdapter {
    public:
        MOCK_METHOD(std::shared_ptr<IDatabaseExecResult>, executeSqlCommand, (const std::string &command), (override));
        MOCK_METHOD(void, commit, (), (override));
    };
}


#endif //COURSEWORKDB_MOCKDATABASEADAPTER_H
