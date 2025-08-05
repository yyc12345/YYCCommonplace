#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/num/op.hpp>

#define OP ::yycc::num::op

namespace yycctest::num::op {

    TEST(NumOp, DivCeil) {
        EXPECT_EQ(OP::div_ceil<uint32_t>(7, 4), UINT32_C(2));
    }

}
