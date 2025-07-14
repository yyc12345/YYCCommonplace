#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/string/op.hpp>

#include <yycc/prelude/core.hpp>

#define OP ::yycc::string::op

namespace yycctest::string::op {

    TEST(StringOp, Printf) {
        auto rv = OP::printf(YYCC_U8("%s == %s"), YYCC_U8("Hello World"), YYCC_U8("Hello, world"));
        EXPECT_EQ(rv, YYCC_U8("Hello World == Hello, world"));
    }

    TEST(StringOp, Replace) {
        // Normal case
        {
            auto rv = OP::replace(YYCC_U8("aabbcc"), YYCC_U8("bb"), YYCC_U8("dd"));
            EXPECT_EQ(rv, YYCC_U8("aaddcc"));
        }
        // No matched expected string
        {
            auto rv = OP::replace(YYCC_U8("aabbcc"), YYCC_U8("zz"), YYCC_U8("yy"));
            EXPECT_EQ(rv, YYCC_U8("aabbcc"));
        }
        // Empty expected string
        {
            auto rv = OP::replace(YYCC_U8("aabbcc"), u8string_view(), YYCC_U8("zz"));
            EXPECT_EQ(rv, YYCC_U8("aabbcc"));
        }
        // Empty replace string
        {
            auto rv = OP::replace(YYCC_U8("aaaabbaa"), YYCC_U8("aa"), YYCC_U8(""));
            EXPECT_EQ(rv, YYCC_U8("bb"));
        }
        // Nested replacing
        {
            auto rv = OP::replace(YYCC_U8("aaxcc"), YYCC_U8("x"), YYCC_U8("yx"));
            EXPECT_EQ(rv, YYCC_U8("aayxcc"));
        }
        // Empty source string
        {
            auto rv = OP::replace(u8string_view(), YYCC_U8(""), YYCC_U8("xy"));
            EXPECT_EQ(rv, YYCC_U8(""));
        }
    }

    TEST(StringOp, Lower) {
        auto rv = OP::to_lower(YYCC_U8("LOWER"));
        EXPECT_EQ(rv, YYCC_U8("lower"));
    }

    TEST(StringOp, Upper) {
        auto rv = OP::to_upper(YYCC_U8("upper"));
        EXPECT_EQ(rv, YYCC_U8("UPPER"));
    }

    TEST(StringOp, Join) {
        std::vector<u8string> datas{YYCC_U8(""), YYCC_U8("1"), YYCC_U8("2"), YYCC_U8("")};
        auto rv = OP::join(datas.begin(), datas.end(), YYCC_U8(", "));
        EXPECT_EQ(rv, YYCC_U8(", 1, 2, "));
    }

    TEST(StringOp, Split) {
        // Normal
        {
            auto rv = OP::split(YYCC_U8(", 1, 2, "), YYCC_U8(", "));
            ASSERT_EQ(rv.size(), 4u);
            EXPECT_EQ(rv[0], YYCC_U8(""));
            EXPECT_EQ(rv[1], YYCC_U8("1"));
            EXPECT_EQ(rv[2], YYCC_U8("2"));
            EXPECT_EQ(rv[3], YYCC_U8(""));
        }
        // No matched delimiter
        {
            auto rv = OP::split(YYCC_U8("test"), YYCC_U8("-"));
            ASSERT_EQ(rv.size(), 1u);
            EXPECT_EQ(rv[0], YYCC_U8("test"));
        }
        // Empty delimiter
        {
            auto rv = OP::split(YYCC_U8("test"), u8string_view());
            ASSERT_EQ(rv.size(), 1u);
            EXPECT_EQ(rv[0], YYCC_U8("test"));
        }
        // Empty source string
        {
            auto rv = OP::split(u8string_view(), YYCC_U8(""));
            ASSERT_EQ(rv.size(), 1u);
            EXPECT_TRUE(rv[0].empty());
        }
    }

} // namespace yycctest::string::op
