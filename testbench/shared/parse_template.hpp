/**
 * \file
 * This file is a template for Parse function testing.
 *
 * As you seen that there is 2 style Parse function locate in main namespace and Rust namespace respectively.
 * Both of them share the exactly same test data sheet.
 * So it is good idea to extract these common part and put them into a place, and include it in respectively testbench file.
 * That what this file does.
 *
 * Before including this template file, you must make sure that:
 * \li Have include <gtest/gtest.h>
 * \li Have include <yycc/prelude/rust.hpp>
 * \li Have define a macro named \c TEST_NS which indicate the testbench namespace passed to gtest.
 * \li Have define a macro with syntax <TT>TEST_SUCCESS(type_t, value, string_value, ...)</TT>.
 * This macro will be called for those success case. \c type_t is the generic type of Parse function.
 * \c value is the expected value after parse and \c string_value is the string value to be parsed.
 * Other arguments should be redirect to corresponding Parse function.
 * \li Have define a macro with syntax <TT>TEST_FAIL(type_t, string_value, ...)</TT>.
 * Opposite with \c TEST_SUCCESS, this macro is for those bad case testing.
 * All arguments have the same meaning with \c TEST_SUCCESS.
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

    TEST_SUCCESS(float, 1.0f, "1.0");
    TEST_SUCCESS(double, 1.0, "1.0");

    TEST_SUCCESS(bool, true, "true");
    TEST_SUCCESS(bool, false, "false");
}

TEST(TEST_NS, Radix) {
    TEST_SUCCESS(u32, UINT32_C(0xffff), "ffff", 16);
    TEST_SUCCESS(u32, UINT32_C(032), "032", 8);
    TEST_SUCCESS(u32, UINT32_C(0B1011), "1011", 2);
}

TEST(TEST_NS, CaseInsensitive) {
    TEST_SUCCESS(bool, true, "tRUE");
}

TEST(TEST_NS, Overflow) {
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

TEST(TEST_NS, BadRadix) {
    TEST_FAIL(u32, "fghj", 16);
    TEST_FAIL(u32, "099", 8);
    TEST_FAIL(u32, "12345", 2);
}

TEST(TEST_NS, InvalidWords) {
    TEST_FAIL(u32, "hello, world!");
    TEST_FAIL(bool, "hello, world!");
}
