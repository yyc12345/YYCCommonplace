#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/string/stringify.hpp>
#include <yycc/string/reinterpret.hpp>

#include <yycc/prelude/rust.hpp>

#define STRINGIFY ::yycc::string::stringify

namespace yycctest::string::stringify {

#define TEST_SUCCESS(type_t, value, string_value, ...) { \
    type_t cache = value; \
    u8string ret = STRINGIFY::stringify<type_t>(cache, ##__VA_ARGS__); \
    EXPECT_EQ(ret, YYCC_U8(string_value)); \
}

    TEST(StringStringify, Common) {
        TEST_SUCCESS(i8, INT8_C(-61), "-61");
        TEST_SUCCESS(u8, UINT8_C(200), "200");
        TEST_SUCCESS(i16, INT16_C(6161), "6161");
        TEST_SUCCESS(u16, UINT16_C(32800), "32800");
        TEST_SUCCESS(i32, INT32_C(61616161), "61616161");
        TEST_SUCCESS(u32, UINT32_C(4294967293), "4294967293");
        TEST_SUCCESS(i64, INT64_C(616161616161), "616161616161");
        TEST_SUCCESS(u64, UINT64_C(9223372036854775807), "9223372036854775807");

        TEST_SUCCESS(float, 1.0f, "1.0", std::chars_format::fixed, 1);
        TEST_SUCCESS(double, 1.0, "1.0", std::chars_format::fixed, 1);

        TEST_SUCCESS(bool, true, "true");
        TEST_SUCCESS(bool, false, "false");
    }

    TEST(StringStringify, Radix) {
        TEST_SUCCESS(u32, UINT32_C(0xffff), "ffff", 16);
        TEST_SUCCESS(u32, UINT32_C(032), "32", 8);
        TEST_SUCCESS(u32, UINT32_C(0B1011), "1011", 2);
    }

}
