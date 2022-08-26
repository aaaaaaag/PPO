//
// Created by denis on 7/16/22.
//

#ifndef COURSEWORKDB_MOCKDATABASEEXECRESULT_H
#define COURSEWORKDB_MOCKDATABASEEXECRESULT_H

#include "IDatabaseExecResult.h"
#include "gmock/gmock.h"

namespace polytour::db::tests::mocks {
    class MockDatabaseExecResult: public IDatabaseExecResult {
    public:
        MOCK_METHOD(std::vector<utility::FieldSet>, toFieldSetCollection, (), (override));
    };
}

#endif //COURSEWORKDB_MOCKDATABASEEXECRESULT_H
