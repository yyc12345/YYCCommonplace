#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/num/parse.hpp>

#include <yycc/prelude/rust.hpp>

#define PARSE ::yycc::num::parse

namespace yycctest::num::parse {

    // These 2 test macros build string container via given string.
    // Check `try_parse` first, and then check `parse`.

#define TEST_NS NumParse

#define TEST_SUCCESS(type_t, value, string_value, ...) \
    { \
        u8string cache_string(YYCC_U8(string_value)); \
        type_t cache; \
        ASSERT_TRUE(PARSE::try_parse<type_t>(cache_string, cache, ##__VA_ARGS__)); \
        EXPECT_EQ(cache, value); \
        EXPECT_EQ(PARSE::parse<type_t>(cache_string, ##__VA_ARGS__), value); \
    }

#define TEST_FAIL(type_t, string_value, ...) \
    { \
        u8string cache_string(YYCC_U8(string_value)); \
        type_t cache; \
        EXPECT_FALSE(PARSE::try_parse<type_t>(cache_string, cache, ##__VA_ARGS__)); \
        EXPECT_ANY_THROW(PARSE::parse<type_t>(cache_string, ##__VA_ARGS__)); \
    }

#include "../../shared/parse_template.hpp"

} // namespace yycctest::num::parse
