#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/string/stream.hpp>
#include <yycc/string/reinterpret.hpp>
#include <sstream>

#define REINTERPRET ::yycc::string::reinterpret
using namespace std::literals::string_view_literals;
using namespace ::yycc::string::stream;

namespace yycctest::string::stream {
    
    TEST(StringStream, StringView) {
        std::stringstream ss;
        ss << u8"hello"sv;
        EXPECT_EQ(REINTERPRET::as_utf8_view(ss.view()), u8"hello"sv);
    }

    TEST(StringStream, CStrPtr) {
        std::stringstream ss;
        ss << u8"hello";
        EXPECT_EQ(REINTERPRET::as_utf8_view(ss.view()), u8"hello");
    }

    TEST(StringStream, Character) {
        std::stringstream ss;
        ss << u8'y';
        EXPECT_EQ(REINTERPRET::as_utf8_view(ss.view()), u8"y");
    }

}
