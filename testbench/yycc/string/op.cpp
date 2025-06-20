#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/string/op.hpp>
#include <yycc/string/reinterpret.hpp>

#define OP ::yycc::string::op

namespace yycctest::string::op {

    TEST(StringOp, Printf) {
        auto rv = OP::printf(YYCC_U8("%s == %s"), YYCC_U8("Hello World"), YYCC_U8("Hello, world"));
        EXPECT_EQ(rv, YYCC_U8("Hello World == Hello, world"));
    }

    TEST(StringOp, Replace) {

    }

    TEST(StringOp, Lower) {
        auto rv = OP::lower(YYCC_U8("LOWER"));
        EXPECT_EQ(rv, YYCC_U8("lower"));
    }

    TEST(StringOp, Upper) {
        auto rv = OP::upper(YYCC_U8("upper"));
        EXPECT_EQ(rv, YYCC_U8("UPPER"));
    }

    TEST(StringOp, Join) {

    }

    TEST(StringOp, Split) {

    }

}
