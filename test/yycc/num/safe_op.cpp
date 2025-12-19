#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/num/safe_op.hpp>
#include <cstdint>
#include <limits>

#include <yycc/prelude.hpp>

#define OP ::yycc::num::safe_op

namespace yycctest::num::safe_op {

    template<typename T>
    constexpr T MAX = std::numeric_limits<T>::max();

    template<typename T>
    constexpr T MIN = std::numeric_limits<T>::min();

#pragma region Wrapping operations

    TEST(NumSafeOp, WrappingAdd) {
        // Unsigned
        EXPECT_EQ(OP::wrapping_add<u32>(200, 55), UINT32_C(255));
        EXPECT_EQ(OP::wrapping_add<u32>(200, MAX<u32>), UINT32_C(199));
        // Signed
        EXPECT_EQ(OP::wrapping_add<i32>(100, 27), INT32_C(127));
        EXPECT_EQ(OP::wrapping_add<i32>(MAX<i32>, 2), MIN<i32> + 1);
    }

    TEST(NumSafeOp, WrappingSub) {
        // Unsigned
        EXPECT_EQ(OP::wrapping_sub<u32>(100, 100), 0);
        EXPECT_EQ(OP::wrapping_sub<u32>(100, MAX<u32>), UINT32_C(101));
        // Signed
        EXPECT_EQ(OP::wrapping_sub<i32>(0, 127), INT32_C(-127));
        EXPECT_EQ(OP::wrapping_sub<i32>(-2, MAX<i32>), MAX<i32>);
    }

    TEST(NumSafeOp, WrappingMul) {
        // Unsigned
        EXPECT_EQ(OP::wrapping_mul<u8>(10, 12), UINT8_C(120));
        EXPECT_EQ(OP::wrapping_mul<u8>(25, 12), UINT8_C(44));
        // Signed
        EXPECT_EQ(OP::wrapping_mul<i32>(10, 12), INT32_C(120));
        EXPECT_EQ(OP::wrapping_mul<i8>(11, 12), INT32_C(-124));
    }

    TEST(NumSafeOp, WrappingDiv) {
        // Unsigned
        EXPECT_EQ(OP::wrapping_div<u32>(100, 10), UINT32_C(10));
        // Signed
        EXPECT_EQ(OP::wrapping_div<i32>(100, 10), INT32_C(10));
        EXPECT_EQ(OP::wrapping_div<i8>(-128, -1), INT32_C(-128));
    }

#pragma endregion

#pragma region Checked operations

    TEST(NumSafeOp, CheckedAdd) {
        // Unsigned
        {
            auto rv = OP::checked_add<u32>(MAX<u32> - 2, 1);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), MAX<u32> - 1);
        }
        {
            auto rv = OP::checked_add<u32>(MAX<u32> - 2, 3);
            EXPECT_FALSE(rv.has_value());
        }
        // Signed
        {
            auto rv = OP::checked_add<i32>(MAX<i32> - 2, 1);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), MAX<i32> - 1);
        }
        {
            auto rv = OP::checked_add<i32>(MAX<i32> - 2, 3);
            EXPECT_FALSE(rv.has_value());
        }
    }

    TEST(NumSafeOp, CheckedSub) {
        // Unsigned
        {
            auto rv = OP::checked_sub<u32>(1, 1);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), 0);
        }
        {
            auto rv = OP::checked_sub<u32>(0, 1);
            EXPECT_FALSE(rv.has_value());
        }
        // Signed
        {
            auto rv = OP::checked_sub<i32>(MIN<i32> + 2, 1);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), MIN<i32> + 1);
        }
        {
            auto rv = OP::checked_sub<i32>(MIN<i32> + 2, 3);
            EXPECT_FALSE(rv.has_value());
        }
    }

    TEST(NumSafeOp, CheckedMul) {
        // Unsigned
        {
            auto rv = OP::checked_mul<u32>(5, 1);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), 5);
        }
        {
            auto rv = OP::checked_mul<u32>(MAX<u32>, 2);
            EXPECT_FALSE(rv.has_value());
        }
        // Signed
        {
            auto rv = OP::checked_mul<i32>(MAX<i32>, 1);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), MAX<i32>);
        }
        {
            auto rv = OP::checked_mul<i32>(MAX<i32>, 2);
            EXPECT_FALSE(rv.has_value());
        }
    }

    TEST(NumSafeOp, CheckedDiv) {
        // Unsigned
        {
            auto rv = OP::checked_div<u32>(128, 2);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), 64);
        }
        {
            auto rv = OP::checked_div<u32>(1, 0);
            EXPECT_FALSE(rv.has_value());
        }
        // Signed
        {
            auto rv = OP::checked_div<i32>(MIN<i32> + 1, -1);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), INT32_C(2147483647));
        }
        {
            auto rv = OP::checked_div<i32>(MIN<i32>, -1);
            EXPECT_FALSE(rv.has_value());
        }
        {
            auto rv = OP::checked_div<i32>(1, 0);
            EXPECT_FALSE(rv.has_value());
        }
    }

#pragma endregion

#pragma region Strict operations

    TEST(NumSafeOp, StrictAdd) {
        // Unsigned
        {
            auto rv = OP::strict_add<u32>(MAX<u32> - 2, 1);
            EXPECT_EQ(rv, MAX<u32> - 1);
        }
        {
            EXPECT_ANY_THROW(OP::strict_add<u32>(MAX<u32> - 2, 3));
        }
        // Signed
        {
            auto rv = OP::strict_add<i32>(MAX<i32> - 2, 1);
            EXPECT_EQ(rv, MAX<i32> - 1);
        }
        {
            EXPECT_ANY_THROW(OP::strict_add<i32>(MAX<i32> - 2, 3));
        }
    }

    TEST(NumSafeOp, StrictSub) {
        // Unsigned
        {
            auto rv = OP::strict_sub<u32>(1, 1);
            EXPECT_EQ(rv, 0);
        }
        {
            EXPECT_ANY_THROW(OP::strict_sub<u32>(0, 1));
        }
        // Signed
        {
            auto rv = OP::strict_sub<i32>(MIN<i32> + 2, 1);
            EXPECT_EQ(rv, MIN<i32> + 1);
        }
        {
            EXPECT_ANY_THROW(OP::strict_sub<i32>(MIN<i32> + 2, 3));
        }
    }

    TEST(NumSafeOp, StrictMul) {
        // Unsigned
        {
            auto rv = OP::strict_mul<u32>(5, 1);
            EXPECT_EQ(rv, 5);
        }
        {
            EXPECT_ANY_THROW(OP::strict_mul<u32>(MAX<u32>, 2));
        }
        // Signed
        {
            auto rv = OP::strict_mul<i32>(MAX<i32>, 1);
            EXPECT_EQ(rv, MAX<i32>);
        }
        {
            EXPECT_ANY_THROW(OP::strict_mul<i32>(MAX<i32>, 2));
        }
    }

    TEST(NumSafeOp, StrictDiv) {
        // Unsigned
        {
            auto rv = OP::strict_div<u32>(128, 2);
            EXPECT_EQ(rv, 64);
        }
        {
            EXPECT_ANY_THROW(OP::strict_div<u32>(1, 0));
        }
        // Signed
        {
            auto rv = OP::strict_div<i32>(MIN<i32> + 1, -1);
            EXPECT_EQ(rv, INT32_C(2147483647));
        }
        {
            EXPECT_ANY_THROW(OP::strict_div<i32>(MIN<i32>, -1));
        }
        {
            EXPECT_ANY_THROW(OP::strict_div<i32>(1, 0));
        }
    }

#pragma endregion

#pragma region Overflowing operations

    TEST(NumSafeOp, OverflowingAdd) {
        // Unsigned
        {
            auto rv = OP::overflowing_add<u32>(5, 2);
            EXPECT_EQ(rv.first, 7);
            EXPECT_EQ(rv.second, false);
        }
        {
            auto rv = OP::overflowing_add<u32>(MAX<u32>, 1);
            EXPECT_EQ(rv.first, 0);
            EXPECT_EQ(rv.second, true);
        }
        // Signed
        {
            auto rv = OP::overflowing_add<i32>(5, 2);
            EXPECT_EQ(rv.first, 7);
            EXPECT_EQ(rv.second, false);
        }
        {
            auto rv = OP::overflowing_add<i32>(MAX<i32>, 1);
            EXPECT_EQ(rv.first, MIN<i32>);
            EXPECT_EQ(rv.second, true);
        }
    }

    TEST(NumSafeOp, OverflowingSub) {
        // Unsigned
        {
            auto rv = OP::overflowing_sub<u32>(5, 2);
            EXPECT_EQ(rv.first, 3);
            EXPECT_EQ(rv.second, false);
        }
        {
            auto rv = OP::overflowing_sub<u32>(0, 1);
            EXPECT_EQ(rv.first, MAX<u32>);
            EXPECT_EQ(rv.second, true);
        }
        // Signed
        {
            auto rv = OP::overflowing_sub<i32>(5, 2);
            EXPECT_EQ(rv.first, 3);
            EXPECT_EQ(rv.second, false);
        }
        {
            auto rv = OP::overflowing_sub<i32>(MIN<i32>, 1);
            EXPECT_EQ(rv.first, MAX<i32>);
            EXPECT_EQ(rv.second, true);
        }
    }

    TEST(NumSafeOp, OverflowingMul) {
        // Unsigned
        {
            auto rv = OP::overflowing_mul<u32>(5, 2);
            EXPECT_EQ(rv.first, 10);
            EXPECT_EQ(rv.second, false);
        }
        {
            auto rv = OP::overflowing_mul<u32>(UINT32_C(1000000000), 10);
            EXPECT_EQ(rv.first, UINT32_C(1410065408));
            EXPECT_EQ(rv.second, true);
        }
        // Signed
        {
            auto rv = OP::overflowing_mul<i32>(5, 2);
            EXPECT_EQ(rv.first, 10);
            EXPECT_EQ(rv.second, false);
        }
        {
            auto rv = OP::overflowing_mul<i32>(INT32_C(1000000000), 10);
            EXPECT_EQ(rv.first, INT32_C(1410065408));
            EXPECT_EQ(rv.second, true);
        }
    }

    TEST(NumSafeOp, OverflowingDiv) {
        // Unsigned
        {
            auto rv = OP::overflowing_div<u32>(5, 2);
            EXPECT_EQ(rv.first, 2);
            EXPECT_EQ(rv.second, false);
        }
        // Signed
        {
            auto rv = OP::overflowing_div<i32>(5, 2);
            EXPECT_EQ(rv.first, 2);
            EXPECT_EQ(rv.second, false);
        }
        {
            auto rv = OP::overflowing_div<i32>(MIN<i32>, -1);
            EXPECT_EQ(rv.first, MIN<i32>);
            EXPECT_EQ(rv.second, true);
        }
    }

#pragma endregion

#pragma region Saturating operations

    TEST(NumSafeOp, SaturatingAdd) {
        // Unsigned
        EXPECT_EQ(OP::saturating_add<u32>(100, 1), UINT32_C(101));
        EXPECT_EQ(OP::saturating_add<u32>(MAX<u32>, 127), MAX<u32>);
        // Signed
        EXPECT_EQ(OP::saturating_add<i32>(100, 1), INT32_C(101));
        EXPECT_EQ(OP::saturating_add<i32>(MAX<i32>, 100), MAX<i32>);
        EXPECT_EQ(OP::saturating_add<i32>(MIN<i32>, -1), MIN<i32>);
    }

    TEST(NumSafeOp, SaturatingSub) {
        // Unsigned
        EXPECT_EQ(OP::saturating_sub<u32>(100, 27), UINT32_C(73));
        EXPECT_EQ(OP::saturating_sub<u32>(13, 127), 0);
        // Signed
        EXPECT_EQ(OP::saturating_sub<i32>(100, 127), -27);
        EXPECT_EQ(OP::saturating_sub<i32>(MIN<i32>, 100), MIN<i32>);
        EXPECT_EQ(OP::saturating_sub<i32>(MAX<i32>, -1), MAX<i32>);
    }

    TEST(NumSafeOp, SaturatingMul) {
        // Unsigned
        EXPECT_EQ(OP::saturating_mul<u32>(2, 10), UINT32_C(20));
        EXPECT_EQ(OP::saturating_mul<u32>(MAX<u32>, 10), MAX<u32>);
        // Signed
        EXPECT_EQ(OP::saturating_mul<i32>(10, 12), 120);
        EXPECT_EQ(OP::saturating_mul<i32>(MAX<i32>, 10), MAX<i32>);
        EXPECT_EQ(OP::saturating_mul<i32>(MIN<i32>, 10), MIN<i32>);
    }

    TEST(NumSafeOp, SaturatingDiv) {
        // Unsigned
        EXPECT_EQ(OP::saturating_div<u32>(5, 2), UINT32_C(2));
        // Signed
        EXPECT_EQ(OP::saturating_div<i32>(5, 2), 2);
        EXPECT_EQ(OP::saturating_div<i32>(MAX<i32>, -1), MIN<i32> + 1);
        EXPECT_EQ(OP::saturating_div<i32>(MIN<i32>, -1), MAX<i32>);
    }

#pragma endregion

} // namespace yycctest::num::safe_op
