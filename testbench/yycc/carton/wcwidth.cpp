#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/carton/wcwidth.hpp>

#define WCWDITH ::yycc::carton::wcwidth

namespace yycctest::carton::wcwidth {

#define TEST_SUCCESS(strl, len) \
    { \
        auto rv = WCWDITH::wcswidth(strl); \
        ASSERT_TRUE(rv.has_value()); \
        EXPECT_EQ(rv.value(), len); \
    }

#define TEST_FAIL(strl) \
    { \
        auto rv = WCWDITH::wcswidth(strl); \
        EXPECT_FALSE(rv.has_value()); \
    }

    TEST(CartonWcwdith, BadAnsi) {
        TEST_FAIL(u8"\033?");
    }

    TEST(CartonWcwdith, BadCsi) {
        TEST_FAIL(u8"\033[\t");
    }

    TEST(CartonWcwdith, English) {
        TEST_SUCCESS(u8"abc", 3);
    }

    TEST(CartonWcwdith, Chinese) {
        TEST_SUCCESS(u8"中文", 4);
        TEST_SUCCESS(u8"中a文", 5);
    }

    TEST(CartonWcwdith, Japanese) {
        TEST_SUCCESS(u8"ありがとう", 10);
        TEST_SUCCESS(u8"アリガトウ", 10);
        TEST_SUCCESS(u8"ｱﾘｶﾞﾄｳ", 6);
    }

    TEST(CartonWcwdith, Termcolor) {
        // TODO: Fix this after finish "termcolor".
        // assert_eq!(wcswidth(&colored("abc", Color::Red, Default::default(), Default::default())), 3);
        // assert_eq!(wcswidth(&colored("中文", Color::Red, Default::default(), Default::default())), 4);
        // assert_eq!(wcswidth(&colored("ありがとう", Color::Red, Default::default(), Default::default())), 10);
    }

} // namespace yycctest::carton::wcwidth
