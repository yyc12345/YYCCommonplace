#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/rust/num/parse.hpp>

#include <yycc/prelude/rust.hpp>

#define PARSE ::yycc::rust::num::parse

namespace yycctest::rust::parse {

    // We only want to test it if C++ support it.
#if defined(YYCC_CPPFEAT_EXPECTED)

    // This namespace is just a wrapper for legacy "parse" module.
    // So the test is just a copy of original implementation.
    // Please update this if original test was updated.

#define TEST_SUCCESS(type_t, expected_value, string_value, ...) \
    { \
        u8string cache_string(YYCC_U8(string_value)); \
        auto rv = PARSE::parse<type_t>(cache_string, ##__VA_ARGS__); \
        ASSERT_TRUE(rv.has_value()); \
        EXPECT_EQ(rv.value(), expected_value); \
    }

#define TEST_FAIL(type_t, string_value, ...) \
    { \
        u8string cache_string(YYCC_U8(string_value)); \
        auto rv = PARSE::parse<type_t>(cache_string, ##__VA_ARGS__); \
        EXPECT_FALSE(rv.has_value()); \
    }

    TEST(RustNumParse, Common) {
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

    TEST(RustNumParse, Radix) {
        TEST_SUCCESS(u32, UINT32_C(0xffff), "ffff", 16);
        TEST_SUCCESS(u32, UINT32_C(032), "032", 8);
        TEST_SUCCESS(u32, UINT32_C(0B1011), "1011", 2);
    }

    TEST(RustNumParse, CaseInsensitive) {
        TEST_SUCCESS(bool, true, "tRUE");
    }

    TEST(RustNumParse, Overflow) {
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

    TEST(RustNumParse, BadRadix) {
        TEST_FAIL(u32, "fghj", 16);
        TEST_FAIL(u32, "099", 8);
        TEST_FAIL(u32, "12345", 2);
    }

    TEST(RustNumParse, InvalidWords) {
        TEST_FAIL(u32, "hello, world!");
        TEST_FAIL(bool, "hello, world!");
    }

#endif

} // namespace yycctest::rust::parse
