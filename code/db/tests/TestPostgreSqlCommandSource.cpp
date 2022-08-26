//
// Created by denis on 7/16/22.
//

#include "gtest/gtest.h"
#include "utility/PostgreSqlCommandSource.h"
#include "CriticalError.h"

namespace polytour::db::tests::utility {

    class PostgreSqlCommandSourceTests: public ::testing::Test {
    protected:
        void SetUp() override {
            _pCommandSource = std::make_shared<decltype(_pCommandSource)::element_type>();

            _repoIdentity.tableName = "test";
            _repoIdentity.tableColumns.emplace("a", repository::ColumnDescription{
                .type = polytour::db::utility::TableTypes::Int,
                .mandatory = polytour::db::utility::FieldMandatory::NotNull
            });
            _repoIdentity.tableColumns.emplace("b", repository::ColumnDescription{
                    .type = polytour::db::utility::TableTypes::Int,
                    .mandatory = polytour::db::utility::FieldMandatory::MayBeNull
            });
            _repoIdentity.tableColumns.emplace("c", repository::ColumnDescription{
                    .type = polytour::db::utility::TableTypes::String,
                    .mandatory = polytour::db::utility::FieldMandatory::NotNull
            });
            _repoIdentity.tableColumns.emplace("d", repository::ColumnDescription{
                    .type = polytour::db::utility::TableTypes::String,
                    .mandatory = polytour::db::utility::FieldMandatory::MayBeNull
            });
        }
        std::shared_ptr<polytour::db::utility::PostgreSqlCommandSource> _pCommandSource;
        polytour::db::repository::Identity _repoIdentity;
    };


    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++            add_obj            ++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    TEST_F(PostgreSqlCommandSourceTests, addObj_negativeInvalidIdentityColumnType) {
        _repoIdentity.tableColumns.emplace("wrong", repository::ColumnDescription{
                .type = polytour::db::utility::TableTypes::Null,
                .mandatory = polytour::db::utility::FieldMandatory::MayBeNull
        });
        db::utility::FieldSet fieldSet;
        fieldSet.addPair("a", db::utility::TableAbstractValue(15));
        fieldSet.addPair("b", db::utility::TableAbstractValue(15));
        fieldSet.addPair("c", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("d", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("wrong", db::utility::TableAbstractValue(0));
        EXPECT_THROW(_pCommandSource->addObj(fieldSet, _repoIdentity), polytour::CriticalError);
    }

    TEST_F(PostgreSqlCommandSourceTests, addObj_negativeDifferColumnsFromIdentity) {
        db::utility::FieldSet fieldSet;
        fieldSet.addPair("a", db::utility::TableAbstractValue(15));
        fieldSet.addPair("b", db::utility::TableAbstractValue(15));
        fieldSet.addPair("c", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("d", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("wrong", db::utility::TableAbstractValue(0));
        EXPECT_THROW(_pCommandSource->addObj(fieldSet, _repoIdentity), polytour::CriticalError);
    }

    TEST_F(PostgreSqlCommandSourceTests, addObj_negativeNullColumnsThatMustBeNotNull) {
        db::utility::FieldSet fieldSet;
        fieldSet.addPair("b", db::utility::TableAbstractValue(15));
        fieldSet.addPair("d", db::utility::TableAbstractValue("15"));
        EXPECT_THROW(_pCommandSource->addObj(fieldSet, _repoIdentity), polytour::CriticalError);
    }

    TEST_F(PostgreSqlCommandSourceTests, addObj_positiveAllValuesAreSet) {
        db::utility::FieldSet fieldSet;
        fieldSet.addPair("a", db::utility::TableAbstractValue(15));
        fieldSet.addPair("b", db::utility::TableAbstractValue(15));
        fieldSet.addPair("c", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("d", db::utility::TableAbstractValue("15"));
        EXPECT_EQ(_pCommandSource->addObj(fieldSet, _repoIdentity),
                  "insert into test (a, b, c, d) values (15, 15, '15', '15')");
    }

    TEST_F(PostgreSqlCommandSourceTests, addObj_positiveNotAllValuesAreSet) {
        db::utility::FieldSet fieldSet;
        fieldSet.addPair("a", db::utility::TableAbstractValue(15));
        fieldSet.addPair("c", db::utility::TableAbstractValue("15"));
            EXPECT_EQ(_pCommandSource->addObj(fieldSet, _repoIdentity),
                      "insert into test (a, b, c, d) values (15, null, '15', null)");
    }

    TEST_F(PostgreSqlCommandSourceTests, addObj_positiveObvioslyNullSetValues) {
        db::utility::FieldSet fieldSet;
        fieldSet.addPair("a", db::utility::TableAbstractValue(15));
        fieldSet.addPair("b", db::utility::TableAbstractValue(std::nullopt));
        fieldSet.addPair("c", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("d", db::utility::TableAbstractValue(std::nullopt));
        EXPECT_EQ(_pCommandSource->addObj(fieldSet, _repoIdentity),
                  "insert into test (a, b, c, d) values (15, null, '15', null)");
    }


    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++          delete_obj            +++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    TEST_F(PostgreSqlCommandSourceTests, deleteObj_negativeInvalidIdentityColumnType) {
        _repoIdentity.tableColumns.emplace("wrong", repository::ColumnDescription{
                .type = polytour::db::utility::TableTypes::Null,
                .mandatory = polytour::db::utility::FieldMandatory::MayBeNull
        });
        db::utility::FieldSet fieldSet;
        fieldSet.addPair("a", db::utility::TableAbstractValue(15));
        fieldSet.addPair("b", db::utility::TableAbstractValue(15));
        fieldSet.addPair("c", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("d", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("wrong", db::utility::TableAbstractValue(0));
        EXPECT_THROW(_pCommandSource->deleteObj(fieldSet, _repoIdentity), polytour::CriticalError);
    }

    TEST_F(PostgreSqlCommandSourceTests, deleteObj_negativeDifferColumnsFromIdentity) {
        db::utility::FieldSet fieldSet;
        fieldSet.addPair("a", db::utility::TableAbstractValue(15));
        fieldSet.addPair("b", db::utility::TableAbstractValue(15));
        fieldSet.addPair("c", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("d", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("wrong", db::utility::TableAbstractValue(0));
        EXPECT_THROW(_pCommandSource->deleteObj(fieldSet, _repoIdentity), polytour::CriticalError);
    }

    TEST_F(PostgreSqlCommandSourceTests, deleteObj_positiveAllValuesAreSet) {
        db::utility::FieldSet fieldSet;
        fieldSet.addPair("a", db::utility::TableAbstractValue(15));
        fieldSet.addPair("b", db::utility::TableAbstractValue(15));
        fieldSet.addPair("c", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("d", db::utility::TableAbstractValue("15"));
        EXPECT_EQ(_pCommandSource->deleteObj(fieldSet, _repoIdentity),
                  "delete from test where a = 15 and b = 15 and c = '15' and d = '15'");
    }

    TEST_F(PostgreSqlCommandSourceTests, deleteObj_positiveNotAllValuesAreSet) {
        db::utility::FieldSet fieldSet;
        fieldSet.addPair("a", db::utility::TableAbstractValue(15));
        fieldSet.addPair("c", db::utility::TableAbstractValue("15"));
        EXPECT_EQ(_pCommandSource->deleteObj(fieldSet, _repoIdentity),
                  "delete from test where a = 15 and c = '15'");
    }

    TEST_F(PostgreSqlCommandSourceTests, deleteObj_positiveSetValuesWithNullValue) {
        db::utility::FieldSet fieldSet;
        fieldSet.addPair("a", db::utility::TableAbstractValue(15));
        fieldSet.addPair("b", db::utility::TableAbstractValue(std::nullopt));
        fieldSet.addPair("c", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("d", db::utility::TableAbstractValue(std::nullopt));
        EXPECT_EQ(_pCommandSource->deleteObj(fieldSet, _repoIdentity),
                  "delete from test where a = 15 and b is null and c = '15' and d is null");
    }


    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++          update_obj            +++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    TEST_F(PostgreSqlCommandSourceTests, updateObj_negativeInvalidIdentityColumnType) {
        _repoIdentity.tableColumns.emplace("wrong", repository::ColumnDescription{
                .type = polytour::db::utility::TableTypes::Null,
                .mandatory = polytour::db::utility::FieldMandatory::MayBeNull
        });

        db::utility::FieldSet origFieldSet;
        origFieldSet.addPair("a", db::utility::TableAbstractValue(15));
        origFieldSet.addPair("b", db::utility::TableAbstractValue(15));
        origFieldSet.addPair("c", db::utility::TableAbstractValue("15"));
        origFieldSet.addPair("d", db::utility::TableAbstractValue("15"));

        db::utility::FieldSet newFieldSet = origFieldSet;
        origFieldSet.addPair("wrong", db::utility::TableAbstractValue(0));
        EXPECT_THROW(_pCommandSource->updateObj(origFieldSet, newFieldSet, _repoIdentity),
                     polytour::CriticalError);
    }

    TEST_F(PostgreSqlCommandSourceTests, updateObj_negativeOrigSetHasDifferColumnsFromIdentity) {
        db::utility::FieldSet origFieldSet;
        origFieldSet.addPair("a", db::utility::TableAbstractValue(15));
        origFieldSet.addPair("b", db::utility::TableAbstractValue(15));
        origFieldSet.addPair("c", db::utility::TableAbstractValue("15"));
        origFieldSet.addPair("d", db::utility::TableAbstractValue("15"));

        db::utility::FieldSet newFieldSet = origFieldSet;

        origFieldSet.addPair("wrong", db::utility::TableAbstractValue(0));

        EXPECT_THROW(_pCommandSource->updateObj(origFieldSet, newFieldSet, _repoIdentity), polytour::CriticalError);
    }

    TEST_F(PostgreSqlCommandSourceTests, updateObj_negativeNewSetHasDifferColumnsFromIdentity) {
        db::utility::FieldSet origFieldSet;
        origFieldSet.addPair("a", db::utility::TableAbstractValue(15));
        origFieldSet.addPair("b", db::utility::TableAbstractValue(15));
        origFieldSet.addPair("c", db::utility::TableAbstractValue("15"));
        origFieldSet.addPair("d", db::utility::TableAbstractValue("15"));

        db::utility::FieldSet newFieldSet = origFieldSet;

        newFieldSet.addPair("wrong", db::utility::TableAbstractValue(0));

        EXPECT_THROW(_pCommandSource->updateObj(origFieldSet, newFieldSet, _repoIdentity), polytour::CriticalError);
    }

    TEST_F(PostgreSqlCommandSourceTests, updateObj_positiveAllValuesAreSet) {
        db::utility::FieldSet origFieldSet;
        origFieldSet.addPair("a", db::utility::TableAbstractValue(15));
        origFieldSet.addPair("b", db::utility::TableAbstractValue(15));
        origFieldSet.addPair("c", db::utility::TableAbstractValue("15"));
        origFieldSet.addPair("d", db::utility::TableAbstractValue("15"));

        db::utility::FieldSet newFieldSet = origFieldSet;

        newFieldSet.updatePair("a", db::utility::TableAbstractValue(16));

        EXPECT_EQ(_pCommandSource->updateObj(origFieldSet, newFieldSet, _repoIdentity),
                  "update test set a = 16, b = 15, c = '15', d = '15' where a = 15 and b = 15 and c = '15' and d = '15'");
    }

    TEST_F(PostgreSqlCommandSourceTests, updateObj_positiveNotAllValuesAreSet) {
        db::utility::FieldSet origFieldSet;
        origFieldSet.addPair("a", db::utility::TableAbstractValue(15));
        origFieldSet.addPair("c", db::utility::TableAbstractValue("15"));

        db::utility::FieldSet newFieldSet = origFieldSet;

        newFieldSet.updatePair("a", db::utility::TableAbstractValue(16));

        EXPECT_EQ(_pCommandSource->updateObj(origFieldSet, newFieldSet, _repoIdentity),
                  "update test set a = 16, b = null, c = '15', d = null where a = 15 and c = '15'");
    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++          search_obj            +++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    TEST_F(PostgreSqlCommandSourceTests, searchObj_negativeInvalidIdentityColumnType) {
        _repoIdentity.tableColumns.emplace("wrong", repository::ColumnDescription{
                .type = polytour::db::utility::TableTypes::Null,
                .mandatory = polytour::db::utility::FieldMandatory::MayBeNull
        });
        db::utility::FieldSet fieldSet;
        fieldSet.addPair("a", db::utility::TableAbstractValue(15));
        fieldSet.addPair("b", db::utility::TableAbstractValue(15));
        fieldSet.addPair("c", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("d", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("wrong", db::utility::TableAbstractValue(0));
        EXPECT_THROW(_pCommandSource->searchObj(fieldSet, _repoIdentity), polytour::CriticalError);
    }

    TEST_F(PostgreSqlCommandSourceTests, searchObj_negativeDifferColumnsFromIdentity) {
        db::utility::FieldSet fieldSet;
        fieldSet.addPair("a", db::utility::TableAbstractValue(15));
        fieldSet.addPair("b", db::utility::TableAbstractValue(15));
        fieldSet.addPair("c", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("d", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("wrong", db::utility::TableAbstractValue(0));

        EXPECT_THROW(_pCommandSource->searchObj(fieldSet, _repoIdentity), polytour::CriticalError);
    }

    TEST_F(PostgreSqlCommandSourceTests, searchObj_positiveAllValuesAreSet) {
        db::utility::FieldSet fieldSet;
        fieldSet.addPair("a", db::utility::TableAbstractValue(15));
        fieldSet.addPair("b", db::utility::TableAbstractValue(15));
        fieldSet.addPair("c", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("d", db::utility::TableAbstractValue("15"));

        EXPECT_EQ(_pCommandSource->searchObj(fieldSet, _repoIdentity),
                  "select * from test where a = 15 and b = 15 and c = '15' and d = '15'");
    }

    TEST_F(PostgreSqlCommandSourceTests, searchObj_positiveNotAllValuesAreSet) {
        db::utility::FieldSet fieldSet;
        fieldSet.addPair("a", db::utility::TableAbstractValue(15));
        fieldSet.addPair("c", db::utility::TableAbstractValue("15"));

        EXPECT_EQ(_pCommandSource->searchObj(fieldSet, _repoIdentity),
                  "select * from test where a = 15 and c = '15'");
    }

    TEST_F(PostgreSqlCommandSourceTests, searchObj_positiveAllValuesAreSetWithNulls) {
        db::utility::FieldSet fieldSet;
        fieldSet.addPair("a", db::utility::TableAbstractValue(15));
        fieldSet.addPair("b", db::utility::TableAbstractValue(std::nullopt));
        fieldSet.addPair("c", db::utility::TableAbstractValue("15"));
        fieldSet.addPair("d", db::utility::TableAbstractValue(std::nullopt));

        EXPECT_EQ(_pCommandSource->searchObj(fieldSet, _repoIdentity),
                  "select * from test where a = 15 and b is null and c = '15' and d is null");
    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++          init            +++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    TEST_F(PostgreSqlCommandSourceTests, init_positiveBaseTest) {
        EXPECT_EQ(_pCommandSource->initTable(_repoIdentity),
                  "create table if not exists test (a int not null, b int, c text not null, d text)");
    }
}