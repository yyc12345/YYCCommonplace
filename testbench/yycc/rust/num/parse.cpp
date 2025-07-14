#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/rust/num/parse.hpp>

#include <yycc/prelude/rust.hpp>

#define PARSE ::yycc::rust::num::parse

namespace yycctest::rust::num::parse {

    // We only want to test it if C++ support it.
#if defined(YYCC_CPPFEAT_EXPECTED)

#define TEST_NS RustNumParse

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

#include "../../../shared/parse_template.hpp"

#endif

} // namespace yycctest::rust::parse
