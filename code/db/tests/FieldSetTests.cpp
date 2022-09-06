//
// Created by denis on 19.07.2022.
//

#include "utility/FieldSet.h"
#include "gtest/gtest.h"

namespace polytour::db::tests::utility {

    class FieldSetTests: public ::testing::Test {
    protected:
        void SetUp() override {
        }
        db::utility::FieldSet fieldSet;
    };

    TEST_F(FieldSetTests, testFuncs) {
        int intValue = 16;
        std::string testKey = "a";
        fieldSet.addPair(testKey, db::utility::TableAbstractValue(intValue));
        EXPECT_TRUE(fieldSet.has_value_under_key(testKey));
        EXPECT_FALSE(fieldSet.has_value_under_key("wrong"));
        EXPECT_EQ(fieldSet[testKey].toInt(), intValue);
        EXPECT_THROW(fieldSet["wrong"], std::invalid_argument);
        int newIntValue = intValue + 1;
        fieldSet.updatePair(testKey, db::utility::TableAbstractValue(newIntValue));
        EXPECT_EQ(fieldSet[testKey].toInt(), newIntValue);
    }

    TEST_F(FieldSetTests, testInterators) {
        int value = 15;
        fieldSet.addPair("a", db::utility::TableAbstractValue(value));
        fieldSet.addPair("b", db::utility::TableAbstractValue(value));
        fieldSet.addPair("c", db::utility::TableAbstractValue(value));
        for (auto& field: fieldSet) {
            auto fieldValue = field.second.toInt();
            field.second = db::utility::TableAbstractValue(fieldValue + 1);
        }
        EXPECT_EQ(fieldSet["a"].toInt(), value + 1);
        EXPECT_EQ(fieldSet["b"].toInt(), value + 1);
        EXPECT_EQ(fieldSet["c"].toInt(), value + 1);
    }
}