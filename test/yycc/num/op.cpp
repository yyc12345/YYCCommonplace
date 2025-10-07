#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/num/op.hpp>

#include <yycc/prelude.hpp>

#define OP ::yycc::num::op

namespace yycctest::num::op {

    TEST(NumOp, DivCeil) {
        // Normal case
        EXPECT_EQ(OP::div_ceil<u32>(8, 4), UINT32_C(2));
        EXPECT_EQ(OP::div_ceil<u32>(7, 4), UINT32_C(2));
        // Limit case
        EXPECT_EQ(OP::div_ceil<u8>(255, 2), UINT8_C(128));
    }

}
