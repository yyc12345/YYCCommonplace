#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/num/safe_cast.hpp>
#include <yycc/macro/ptr_size_detector.hpp>

#include <yycc/rust/prelude.hpp>

#define CAST ::yycc::num::safe_cast

namespace yycctest::num::safe_cast {

    TEST(NumSafeCast, To) {
        // Definitely okey
        auto rv = CAST::to<u32, u8>(UINT8_C(1));
        EXPECT_EQ(rv, UINT32_C(1));
    }

    TEST(NumSafeCast, TryTo) {
        // Okey
        {
            auto rv = CAST::try_to<u8, u32>(UINT32_C(1));
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), UINT8_C(1));
        }
        // Bad cast
        {
            auto rv = CAST::try_to<u8, u32>(UINT32_C(6161));
            EXPECT_FALSE(rv.has_value());
        }
    }

    TEST(NumSafeCast, VariableLength) {
        // Both 32-bit and 64-bit pointer size are okey.
        {
            auto rv = CAST::try_to<usize, u64>(UINT64_C(0x00000000ffffffff));
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), 0xffffffffu);
        }
        // Only 64-bit pointer size is okey.
        {
            auto rv = CAST::try_to<usize, u64>(UINT64_C(0xffffffffffffffff));
#if defined(YYCC_PTRSIZE_64)
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), 0xffffffffffffffffu);
#else
            EXPECT_FALSE(rv.has_value());
#endif
        }
    }

}
