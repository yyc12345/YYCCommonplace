/**
 * \file
 * This file is a template for Stringify function testing.
 *
 * Same as parse_template.hpp .
 *
 * Before including this template file, you must make sure that:
 * \li Have include <gtest/gtest.h>
 * \li Have include <yycc/prelude/rust.hpp>
 * \li Have define a macro named \c TEST_NS which indicate the testbench namespace passed to gtest.
 * \li Have define a macro with syntax <TT>TEST_SUCCESS(type_t, value, string_value, ...)</TT>.
 * This macro will be called for those success case. \c type_t is the generic type of Stringify function.
 * \c value is the value will be stringified and \c string_value is the expected string.
 * Other arguments should be redirect to corresponding Stringify function.
 *
*/

TEST(TEST_NS, Common) {
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

TEST(TEST_NS, Radix) {
    TEST_SUCCESS(u32, UINT32_C(0xffff), "ffff", 16);
    TEST_SUCCESS(u32, UINT32_C(032), "32", 8);
    TEST_SUCCESS(u32, UINT32_C(0B1011), "1011", 2);
}
