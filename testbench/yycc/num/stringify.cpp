#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/num/stringify.hpp>

#include <yycc/rust/prelude.hpp>

#define STRINGIFY ::yycc::num::stringify

namespace yycctest::num::stringify {

#define TEST_SUCCESS(type_t, value, string_value, ...) \
    { \
        type_t cache = value; \
        std::u8string ret = STRINGIFY::stringify<type_t>(cache __VA_OPT__(,) __VA_ARGS__); \
        EXPECT_EQ(ret, string_value); \
    }

    TEST(NumStringify, Common) {
        TEST_SUCCESS(i8, INT8_C(-61), u8"-61");
        TEST_SUCCESS(u8, UINT8_C(200), u8"200");
        TEST_SUCCESS(i16, INT16_C(6161), u8"6161");
        TEST_SUCCESS(u16, UINT16_C(32800), u8"32800");
        TEST_SUCCESS(i32, INT32_C(61616161), u8"61616161");
        TEST_SUCCESS(u32, UINT32_C(4294967293), u8"4294967293");
        TEST_SUCCESS(i64, INT64_C(616161616161), u8"616161616161");
        TEST_SUCCESS(u64, UINT64_C(9223372036854775807), u8"9223372036854775807");

        TEST_SUCCESS(float, 1.0f, u8"1.0", std::chars_format::fixed, 1);
        TEST_SUCCESS(double, 1.0, u8"1.0", std::chars_format::fixed, 1);

        TEST_SUCCESS(bool, true, u8"true");
        TEST_SUCCESS(bool, false, u8"false");
    }

    TEST(NumStringify, Radix) {
        TEST_SUCCESS(u32, UINT32_C(0xffff), u8"ffff", 16);
        TEST_SUCCESS(u32, UINT32_C(032), u8"32", 8);
        TEST_SUCCESS(u32, UINT32_C(0B1011), u8"1011", 2);
    }

} // namespace yycctest::num::stringify
