#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/patch/starts_ends_with.hpp>

#define FN_STARTS_WITH ::yycc::patch::starts_ends_with::starts_with
#define FN_ENDS_WITH ::yycc::patch::starts_ends_with::ends_with
using namespace std::literals;

namespace yycctest::patch::starts_ends_with {

#define TEST_STARTS_WITH(prefix) \
    { \
        const auto that = prefix##"Hello, C++20!"s; \
        EXPECT_TRUE(FN_STARTS_WITH(that, prefix##"He"sv)); \
        EXPECT_FALSE(FN_STARTS_WITH(that, prefix##"he"sv)); \
        EXPECT_TRUE(FN_STARTS_WITH(that, prefix##'H')); \
        EXPECT_FALSE(FN_STARTS_WITH(that, prefix##'h')); \
        EXPECT_TRUE(FN_STARTS_WITH(that, prefix##"He")); \
        EXPECT_FALSE(FN_STARTS_WITH(that, prefix##"he")); \
    }

    TEST(PatchStartsEndsWith, StartsWith) {
        // Ordinary string
        TEST_STARTS_WITH();
        // Wide string
        TEST_STARTS_WITH(L);
        // UTF8 string
        TEST_STARTS_WITH(u8);
        // UTF-16 string
        TEST_STARTS_WITH(u);
        // UTF-32 string
        TEST_STARTS_WITH(U);
    }

#define TEST_ENDS_WITH(prefix) \
    { \
        const auto that = prefix##"Hello, C++20!"s; \
        EXPECT_TRUE(FN_ENDS_WITH(that, prefix##"C++20!"sv)); \
        EXPECT_FALSE(FN_ENDS_WITH(that, prefix##"c++20!"sv)); \
        EXPECT_TRUE(FN_ENDS_WITH(that, prefix##'!')); \
        EXPECT_FALSE(FN_ENDS_WITH(that, prefix##'?')); \
        EXPECT_TRUE(FN_ENDS_WITH(that, prefix##"C++20!")); \
        EXPECT_FALSE(FN_ENDS_WITH(that, prefix##"c++20!")); \
    }

    TEST(PatchStartsEndsWith, EndsWith) {
        // Ordinary string
        TEST_ENDS_WITH();
        // Wide string
        TEST_ENDS_WITH(L);
        // UTF8 string
        TEST_ENDS_WITH(u8);
        // UTF-16 string
        TEST_ENDS_WITH(u);
        // UTF-32 string
        TEST_ENDS_WITH(U);
    }

} // namespace yycctest::patch::starts_ends_with
