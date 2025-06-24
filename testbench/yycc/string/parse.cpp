#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/string/parse.hpp>
#include <yycc/string/reinterpret.hpp>

#include <yycc/prelude/rust.hpp>

#define PARSE ::yycc::string::parse

namespace yycctest::string::parse {

    // These 2 test macros build string container via given string.
    // Check `try_parse` first, and then check `parse`.

#define TEST_SUCCESS(type_t, value, string_value, ...) { \
    u8string cache_string(YYCC_U8(string_value)); \
    type_t cache; \
    ASSERT_TRUE(PARSE::try_parse<type_t>(cache_string, cache, ##__VA_ARGS__)); \
    EXPECT_EQ(cache, value); \
    EXPECT_EQ(PARSE::parse<type_t>(cache_string, ##__VA_ARGS__), value); \
}

#define TEST_FAIL(type_t, string_value, ...) { \
    u8string cache_string(YYCC_U8(string_value)); \
    type_t cache; \
    EXPECT_FALSE(PARSE::try_parse<type_t>(cache_string, cache, ##__VA_ARGS__)); \
    EXPECT_ANY_THROW(PARSE::parse<type_t>(cache_string, ##__VA_ARGS__)); \
}

    TEST(StringParse, Common) {
        TEST_SUCCESS(i8, INT8_C(-61), "-61");
        TEST_SUCCESS(u8, UINT8_C(200), "200");
        TEST_SUCCESS(i16, INT16_C(6161), "6161");
        TEST_SUCCESS(u16, UINT16_C(32800), "32800");
        TEST_SUCCESS(i32, INT32_C(61616161), "61616161");
        TEST_SUCCESS(u32, UINT32_C(4294967293), "4294967293");
        TEST_SUCCESS(i64, INT64_C(616161616161), "616161616161");
        TEST_SUCCESS(u64, UINT64_C(9223372036854775807), "9223372036854775807");

        TEST_SUCCESS(float, 1.0f, "1.0");
        TEST_SUCCESS(double, 1.0, "1.0");

        TEST_SUCCESS(bool, true, "true");
        TEST_SUCCESS(bool, false, "false");
    }

    TEST(StringParse, Radix) {
        TEST_SUCCESS(u32, UINT32_C(0xffff), "ffff", 16);
        TEST_SUCCESS(u32, UINT32_C(032), "032", 8);
        TEST_SUCCESS(u32, UINT32_C(0B1011), "1011", 2);
    }

    TEST(StringParse, CaseInsensitive) {
        TEST_SUCCESS(bool, true, "tRUE");
    }

    TEST(StringParse, Overflow) {
        TEST_FAIL(i8, "6161");
        TEST_FAIL(u8, "32800");
        TEST_FAIL(i16, "61616161");
        TEST_FAIL(u16, "4294967293");
        TEST_FAIL(i32, "616161616161");
        TEST_FAIL(u32, "9223372036854775807");
        TEST_FAIL(i64, "616161616161616161616161");
        TEST_FAIL(u64, "92233720368547758079223372036854775807");

        TEST_FAIL(float, "1e40");
        TEST_FAIL(double, "1e114514");
    }

    TEST(StringParse, BadRadix) {
        TEST_FAIL(u32, "fghj", 16);
        TEST_FAIL(u32, "099", 8);
        TEST_FAIL(u32, "12345", 2);
    }

    TEST(StringParse, InvalidWords) {
        TEST_FAIL(u32, "hello, world!");
        TEST_FAIL(bool, "hello, world!");
    }

}
