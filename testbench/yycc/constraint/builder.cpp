#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/constraint/builder.hpp>
#include <yycc/string/reinterpret.hpp>

#include <yycc/prelude/rust.hpp>

#define BUILDER ::yycc::constraint::builder

namespace yycctest::constraint::builder {

#define TEST_SUCCESS(constraint, value) \
    EXPECT_TRUE(constraint.check(value)); \
    EXPECT_EQ(constraint.clamp(value), value);

#define TEST_FAIL(constraint, value, clamped_value) \
    EXPECT_FALSE(constraint.check(value)); \
    EXPECT_EQ(constraint.clamp(value), clamped_value);

    TEST(ConstraintBuilder, MinMaxConstraint) {
        // Integral type
        {
            auto c = BUILDER::min_max_constraint<i32>(5, 61);
            ASSERT_TRUE(c.support_check());
            ASSERT_TRUE(c.support_clamp());
            TEST_SUCCESS(c, 5);
            TEST_SUCCESS(c, 6);
            TEST_SUCCESS(c, 61);
            TEST_FAIL(c, -2, 5);
            TEST_FAIL(c, 0, 5);
            TEST_FAIL(c, 66, 61);
        }
        // Unisgned integral type
        {
            auto c = BUILDER::min_max_constraint<u32>(5, 61);
            ASSERT_TRUE(c.support_check());
            ASSERT_TRUE(c.support_clamp());
            TEST_SUCCESS(c, 5);
            TEST_SUCCESS(c, 6);
            TEST_SUCCESS(c, 61);
            TEST_FAIL(c, 0, 5);
            TEST_FAIL(c, 66, 61);
        }
        // Float point type
        {
            auto c = BUILDER::min_max_constraint<f32>(5.0f, 61.0f);
            ASSERT_TRUE(c.support_check());
            ASSERT_TRUE(c.support_clamp());
            TEST_SUCCESS(c, 5.0f);
            TEST_SUCCESS(c, 6.0f);
            TEST_SUCCESS(c, 61.0f);
            TEST_FAIL(c, 0.0f, 5.0f);
            TEST_FAIL(c, 66.0f, 61.0f);
        }
    }

    enum class TestEnum : u8 { Entry1 = 0, Entry2 = 1, Entry3 = 2 };

    TEST(ConstraintBuilder, EnumConstraint) {
        auto c = BUILDER::enum_constraint({TestEnum::Entry1, TestEnum::Entry2, TestEnum::Entry3},
                                          1u);
        ASSERT_TRUE(c.support_check());
        ASSERT_TRUE(c.support_clamp());
        TEST_SUCCESS(c, TestEnum::Entry1);
        TEST_SUCCESS(c, TestEnum::Entry2);
        TEST_SUCCESS(c, TestEnum::Entry3);
        TEST_FAIL(c, static_cast<TestEnum>(UINT8_C(61)), TestEnum::Entry2);
    }

    TEST(ConstraintBuilder, StrEnumConstraint) {
        auto c = BUILDER::strenum_constraint({YYCC_U8("first-entry"),
                                              YYCC_U8("second-entry"),
                                              YYCC_U8("third-entry")},
                                             1u);
        ASSERT_TRUE(c.support_check());
        ASSERT_TRUE(c.support_clamp());
        TEST_SUCCESS(c, YYCC_U8("first-entry"));
        TEST_SUCCESS(c, YYCC_U8("second-entry"));
        TEST_SUCCESS(c, YYCC_U8("third-entry"));
        TEST_FAIL(c, YYCC_U8("wtf?"), YYCC_U8("second-entry"));
    }

} // namespace yycctest::constraint::builder
