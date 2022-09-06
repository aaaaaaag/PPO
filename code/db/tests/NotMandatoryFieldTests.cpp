//
// Created by denis on 7/19/22.
//

#include "NotMandatoryField.h"
#include "gtest/gtest.h"
#include "CriticalError.h"

namespace polytour::db::tests::utility {

    TEST(NotMandatoryField, nullValue) {
        auto field = transport::utility::NotMandatoryField<int>(std::nullopt);
        EXPECT_FALSE(field.hasValue());
        EXPECT_THROW(field.getValue(), polytour::CriticalError);
        EXPECT_TRUE(field.isNull());
        EXPECT_NO_THROW(field.toNull());
    }

    TEST(NotMandatoryField, withValue) {
        auto field = transport::utility::NotMandatoryField<int>(5);
        EXPECT_TRUE(field.hasValue());
        EXPECT_NO_THROW(field.getValue());
        EXPECT_FALSE(field.isNull());
        EXPECT_THROW(field.toNull(), polytour::CriticalError);
    }
}