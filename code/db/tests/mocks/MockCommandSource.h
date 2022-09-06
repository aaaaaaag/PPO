//
// Created by denis on 7/16/22.
//

#ifndef COURSEWORKDB_MOCKCOMMANDSOURCE_H
#define COURSEWORKDB_MOCKCOMMANDSOURCE_H

#include "utility/ICommandSource.h"
#include "gmock/gmock.h"

namespace polytour::db::tests::mocks {
    class MockCommandSource: public utility::ICommandSource {
    public:

        MOCK_METHOD(std::string, addObj, (const utility::FieldSet &fieldSet, repository::Identity repoIdentity), (override));

        MOCK_METHOD(std::string, deleteObj, (const utility::FieldSet &fieldSet, repository::Identity repoIdentity), (override));

        MOCK_METHOD(std::string, updateObj, (const utility::FieldSet &curFieldSet, const utility::FieldSet &newFieldSet,
                              repository::Identity repoIdentity), (override));

        MOCK_METHOD(std::string, searchObj, (const utility::FieldSet &fieldSet, repository::Identity repoIdentity), (override));

        MOCK_METHOD(std::string, initTable, (const repository::Identity& identity), (override));

    };
}

#endif //COURSEWORKDB_MOCKCOMMANDSOURCE_H
