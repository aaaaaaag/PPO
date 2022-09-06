//
// Created by denis on 7/19/22.
//

#include "utility/TableAbstractValue.h"
#include "gtest/gtest.h"
#include "CriticalError.h"

namespace polytour::db::tests::utility {

TEST(TableAbstractValueTests, StringConstructor) {
    auto strValue = "string";
    auto abstractValue = db::utility::TableAbstractValue(strValue);
    EXPECT_TRUE(abstractValue.isString());
    EXPECT_FALSE(abstractValue.isInt());
    EXPECT_FALSE(abstractValue.isNull());
    EXPECT_EQ(abstractValue.toString(), strValue);
    EXPECT_THROW(abstractValue.toInt(), polytour::CriticalError);
    EXPECT_THROW(abstractValue.toNull(), polytour::CriticalError);
}

TEST(TableAbstractValueTests, IntConstructor) {
    auto intValue = 5;
    auto abstractValue = db::utility::TableAbstractValue(intValue);
    EXPECT_FALSE(abstractValue.isString());
    EXPECT_TRUE(abstractValue.isInt());
    EXPECT_FALSE(abstractValue.isNull());
    EXPECT_EQ(abstractValue.toInt(), intValue);
    EXPECT_THROW(abstractValue.toString(), polytour::CriticalError);
    EXPECT_THROW(abstractValue.toNull(), polytour::CriticalError);
}

TEST(TableAbstractValueTests, NullConstructor) {
    auto nullValue = std::nullopt;
    auto abstractValue = db::utility::TableAbstractValue(nullValue);
    EXPECT_FALSE(abstractValue.isString());
    EXPECT_FALSE(abstractValue.isInt());
    EXPECT_TRUE(abstractValue.isNull());
    EXPECT_NO_THROW(abstractValue.toNull());
    EXPECT_THROW(abstractValue.toString(), polytour::CriticalError);
    EXPECT_THROW(abstractValue.toInt(), polytour::CriticalError);
}

TEST(TableAbstractValueTests, UniversalConstructor) {
    auto nullValue = std::nullopt;
    auto abstractValue = db::utility::TableAbstractValue(nullValue, db::utility::TableTypes::Null);
    EXPECT_FALSE(abstractValue.isString());
    EXPECT_FALSE(abstractValue.isInt());
    EXPECT_TRUE(abstractValue.isNull());
    EXPECT_NO_THROW(abstractValue.toNull());
    EXPECT_THROW(abstractValue.toString(), polytour::CriticalError);
    EXPECT_THROW(abstractValue.toInt(), polytour::CriticalError);
}

//TODO Add negative tests for universal constructor

}