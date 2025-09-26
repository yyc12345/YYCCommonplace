#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/string/op.hpp>

#include <yycc/rust/prelude.hpp>

#define OP ::yycc::string::op
using namespace std::literals::string_view_literals;

namespace yycctest::string::op {

    TEST(StringOp, Printf) {
        // UTF8 string
        {
            auto rv = OP::printf(u8"%s == %s", u8"Hello World", u8"Hello, world");
            EXPECT_EQ(rv, u8"Hello World == Hello, world");
        }
        // Ordinary string
        {
            auto rv = OP::printf("%s == %s", "Hello World", "Hello, world");
            EXPECT_EQ(rv, "Hello World == Hello, world");
        }
    }

    TEST(StringOp, Replace) {
        // Normal case
        {
            auto rv = OP::replace(u8"aabbcc", u8"bb", u8"dd");
            EXPECT_EQ(rv, u8"aaddcc");
        }
        // No matched expected string
        {
            auto rv = OP::replace(u8"aabbcc", u8"zz", u8"yy");
            EXPECT_EQ(rv, u8"aabbcc");
        }
        // Empty expected string
        {
            auto rv = OP::replace(u8"aabbcc", std::u8string_view(), u8"zz");
            EXPECT_EQ(rv, u8"aabbcc");
        }
        // Empty replace string
        {
            auto rv = OP::replace(u8"aaaabbaa", u8"aa", u8"");
            EXPECT_EQ(rv, u8"bb");
        }
        // Nested replacing
        {
            auto rv = OP::replace(u8"aaxcc", u8"x", u8"yx");
            EXPECT_EQ(rv, u8"aayxcc");
        }
        // Empty source string
        {
            auto rv = OP::replace(std::u8string_view(), u8"", u8"xy");
            EXPECT_EQ(rv, u8"");
        }
    }

    TEST(StringOp, Lower) {
        auto rv = OP::to_lower(u8"LOWER");
        EXPECT_EQ(rv, u8"lower");
    }

    TEST(StringOp, Upper) {
        auto rv = OP::to_upper(u8"upper");
        EXPECT_EQ(rv, u8"UPPER");
    }

    TEST(StringOp, Join) {
        std::vector<std::u8string_view> datas{u8""sv, u8"1"sv, u8"2"sv, u8""sv};
        auto rv = OP::join(datas.begin(), datas.end(), u8", ");
        EXPECT_EQ(rv, u8", 1, 2, ");
    }

    TEST(StringOp, Strip) {
        // Normal strip
        {
            auto rv = OP::to_strip(u8" \taaa\n", u8" \t\r\n");
            EXPECT_EQ(rv, u8"aaa");
        }

        // Special strip
        {
            auto rv = OP::to_strip(u8"가가가aaaㄲㄲㄲ", u8"가ㄲ");
            EXPECT_EQ(rv, u8"aaa");
        }
        {
            auto rv = OP::to_strip(u8"가가가aaaㄲㄲㄲ", u8"가");
            EXPECT_EQ(rv, u8"aaaㄲㄲㄲ");
        }
        {
            auto rv = OP::to_strip(u8"가가가aaaㄲㄲㄲ", u8"ㄲ");
            EXPECT_EQ(rv, u8"가가가aaa");
        }
    }

    TEST(StringOp, Split) {
        // Normal
        {
            auto rv = OP::split(u8", 1, 2, ", u8", ");
            ASSERT_EQ(rv.size(), 4u);
            EXPECT_EQ(rv[0], u8"");
            EXPECT_EQ(rv[1], u8"1");
            EXPECT_EQ(rv[2], u8"2");
            EXPECT_EQ(rv[3], u8"");
        }
        // No matched delimiter
        {
            auto rv = OP::split(u8"test", u8"-");
            ASSERT_EQ(rv.size(), 1u);
            EXPECT_EQ(rv[0], u8"test");
        }
        // Empty delimiter
        {
            auto rv = OP::split(u8"test", std::u8string_view());
            ASSERT_EQ(rv.size(), 1u);
            EXPECT_EQ(rv[0], u8"test");
        }
        // Empty source string
        {
            auto rv = OP::split(std::u8string_view(), u8"");
            ASSERT_EQ(rv.size(), 1u);
            EXPECT_TRUE(rv[0].empty());
        }
    }

} // namespace yycctest::string::op
