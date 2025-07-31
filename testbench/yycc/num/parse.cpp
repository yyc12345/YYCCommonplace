#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/num/parse.hpp>

#include <yycc/rust/prelude.hpp>

#define PARSE ::yycc::num::parse

namespace yycctest::num::parse {

    // These 2 test macros build string container via given string.
    // Check `try_parse` first, and then check `parse`.

#define TEST_SUCCESS(type_t, expected_value, string_value, ...) \
    { \
        std::u8string cache_string(string_value); \
        auto rv = PARSE::parse<type_t>(cache_string __VA_OPT__(, ) __VA_ARGS__); \
        ASSERT_TRUE(rv.has_value()); \
        EXPECT_EQ(rv.value(), expected_value); \
    }

#define TEST_FAIL(type_t, string_value, ...) \
    { \
        std::u8string cache_string(string_value); \
        auto rv = PARSE::parse<type_t>(cache_string __VA_OPT__(, ) __VA_ARGS__); \
        EXPECT_FALSE(rv.has_value()); \
    }

    TEST(NumParse, Common) {
        TEST_SUCCESS(i8, INT8_C(-61), u8"-61");
        TEST_SUCCESS(u8, UINT8_C(200), u8"200");
        TEST_SUCCESS(i16, INT16_C(6161), u8"6161");
        TEST_SUCCESS(u16, UINT16_C(32800), u8"32800");
        TEST_SUCCESS(i32, INT32_C(61616161), u8"61616161");
        TEST_SUCCESS(u32, UINT32_C(4294967293), u8"4294967293");
        TEST_SUCCESS(i64, INT64_C(616161616161), u8"616161616161");
        TEST_SUCCESS(u64, UINT64_C(9223372036854775807), u8"9223372036854775807");

        TEST_SUCCESS(float, 1.0f, u8"1.0");
        TEST_SUCCESS(double, 1.0, u8"1.0");

        TEST_SUCCESS(bool, true, u8"true");
        TEST_SUCCESS(bool, false, u8"false");
    }

    TEST(NumParse, Radix) {
        TEST_SUCCESS(u32, UINT32_C(0xffff), u8"ffff", 16);
        TEST_SUCCESS(u32, UINT32_C(032), u8"032", 8);
        TEST_SUCCESS(u32, UINT32_C(0B1011), u8"1011", 2);
    }

    TEST(NumParse, CaseInsensitive) {
        TEST_SUCCESS(bool, true, u8"tRUE");
    }

    TEST(NumParse, Overflow) {
        TEST_FAIL(i8, u8"6161");
        TEST_FAIL(u8, u8"32800");
        TEST_FAIL(i16, u8"61616161");
        TEST_FAIL(u16, u8"4294967293");
        TEST_FAIL(i32, u8"616161616161");
        TEST_FAIL(u32, u8"9223372036854775807");
        TEST_FAIL(i64, u8"616161616161616161616161");
        TEST_FAIL(u64, u8"92233720368547758079223372036854775807");

        TEST_FAIL(float, u8"1e40");
        TEST_FAIL(double, u8"1e114514");
    }

    TEST(NumParse, BadRadix) {
        TEST_FAIL(u32, u8"fghj", 16);
        TEST_FAIL(u32, u8"099", 8);
        TEST_FAIL(u32, u8"12345", 2);
    }

    TEST(NumParse, InvalidWords) {
        TEST_FAIL(u32, u8"hello, world!");
        TEST_FAIL(bool, u8"hello, world!");
    }

} // namespace yycctest::num::parse
