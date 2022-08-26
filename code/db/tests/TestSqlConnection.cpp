//
// Created by denis on 7/16/22.
//

#include "mocks/MockDatabaseAdapter.h"
#include "mocks/MockDatabaseExecResult.h"
#include "mocks/MockCommandSource.h"
#include "SqlConnection.h"

#include <utility>
#include "gtest/gtest.h"

namespace polytour::db::tests {

    using ::testing::Return;

    class SqlConnectionAdapterChanger: public SqlConnection {
        void changeDbAdapter(std::shared_ptr<IDatabaseAdapter> adapter) {
            _pDbAdapter = std::move(adapter);
        }
    };


    class SqlConnectionTests: public ::testing::Test {
    protected:
        void SetUp() override {
            _pDatabaseAdapter = std::make_shared<decltype(_pDatabaseAdapter)::element_type>();
            _pDatabaseExecResult = std::make_shared<decltype(_pDatabaseExecResult)::element_type>();
            _pCommandSource = std::make_shared<decltype(_pCommandSource)::element_type>();
            _pConnection = std::make_shared<decltype(_pConnection)::element_type>(_pDatabaseAdapter, _pCommandSource);
        }

        std::shared_ptr<mocks::MockDatabaseAdapter> _pDatabaseAdapter;
        std::shared_ptr<mocks::MockDatabaseExecResult> _pDatabaseExecResult;
        std::shared_ptr<mocks::MockCommandSource> _pCommandSource;
        std::shared_ptr<SqlConnection> _pConnection;

    };

    TEST_F(SqlConnectionTests, addObj) {
        EXPECT_CALL(*_pCommandSource, addObj);
        EXPECT_CALL(*_pDatabaseAdapter, executeSqlCommand);
        _pConnection->addObj(utility::FieldSet{}, repository::Identity{});
    }

    TEST_F(SqlConnectionTests, deleteObj) {
        EXPECT_CALL(*_pCommandSource, deleteObj);
        EXPECT_CALL(*_pDatabaseAdapter, executeSqlCommand);
        _pConnection->deleteObj(utility::FieldSet{}, repository::Identity{});
    }

    TEST_F(SqlConnectionTests, updateObj) {
        EXPECT_CALL(*_pCommandSource, updateObj);
        EXPECT_CALL(*_pDatabaseAdapter, executeSqlCommand);
        _pConnection->updateObj(utility::FieldSet{},
                                utility::FieldSet{},
                                repository::Identity{});
    }

    TEST_F(SqlConnectionTests, searchObj) {
        EXPECT_CALL(*_pCommandSource, searchObj);
        EXPECT_CALL(*_pDatabaseAdapter, executeSqlCommand).WillOnce(Return(_pDatabaseExecResult));
        EXPECT_CALL(*_pDatabaseExecResult, toFieldSetCollection);
        _pConnection->searchObj(utility::FieldSet{}, repository::Identity{});
    }
}