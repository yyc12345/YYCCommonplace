#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/rust/num/stringify.hpp>

#include <yycc/prelude/rust.hpp>

#define STRINGIFY ::yycc::rust::num::stringify

namespace yycctest::rust::num::stringify {

#define TEST_NS RustNumStringify

#define TEST_SUCCESS(type_t, value, string_value, ...) \
    { \
        type_t cache = value; \
        u8string ret = STRINGIFY::stringify<type_t>(cache, ##__VA_ARGS__); \
        EXPECT_EQ(ret, YYCC_U8(string_value)); \
    }

#include "../../../shared/stringify_template.hpp"

}
