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
            auto rv = OP::strip(u8" \taaa\n", u8" \t\r\n");
            EXPECT_EQ(rv, u8"aaa");
        }
        {
            auto rv = OP::lstrip(u8" \taaa\n", u8" \t\r\n");
            EXPECT_EQ(rv, u8"aaa\n");
        }
        {
            auto rv = OP::rstrip(u8" \taaa\n", u8" \t\r\n");
            EXPECT_EQ(rv, u8" \taaa");
        }

        // Full strip
        {
            auto rv = OP::strip(u8"   ", u8" ");
            EXPECT_TRUE(rv.empty());
        }

        // Special strip
        {
            auto rv = OP::strip(u8"啊啊啊aaaあああ", u8"啊あ");
            EXPECT_EQ(rv, u8"aaa");
        }
        {
            auto rv = OP::strip(u8"啊啊啊aaaあああ", u8"啊");
            EXPECT_EQ(rv, u8"aaaあああ");
        }
        {
            auto rv = OP::strip(u8"啊啊啊aaaあああ", u8"あ");
            EXPECT_EQ(rv, u8"啊啊啊aaa");
        }

        // Possible buggy strip.
        // We use 2 UTF8 code points introduced following:
        // U+00AA  (UTF-8: C2 AA)
        // U+1002A (UTF-8 : F0 90 80 AA)
        {
            auto rv = OP::rstrip(u8"aaa\u00AA", u8"\u00AA\U0001002A");
            EXPECT_EQ(rv, u8"aaa");
        }
    }

    TEST(StringOp, Trim) {
        // Normal trim
        {
            auto rv = OP::trim(u8" \taaa\n", u8" \t\r\n");
            EXPECT_EQ(rv, u8"aaa");
        }
        {
            auto rv = OP::ltrim(u8" \taaa\n", u8" \t\r\n");
            EXPECT_EQ(rv, u8"aaa\n");
        }
        {
            auto rv = OP::rtrim(u8" \taaa\n", u8" \t\r\n");
            EXPECT_EQ(rv, u8" \taaa");
        }

        // Bad words
        {
            EXPECT_ANY_THROW(OP::trim(u8"q啊啊啊aaaあああp", u8"p啊q"));
        }

        // Full trim
        {
            auto rv = OP::trim(u8"   ", u8" ");
            EXPECT_TRUE(rv.empty());
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
