#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/patch/stream.hpp>
#include <yycc/string/reinterpret.hpp>
#include <sstream>

#define REINTERPRET ::yycc::string::reinterpret
using namespace std::literals::string_view_literals;
using namespace ::yycc::patch::stream;

namespace yycctest::patch::stream {
    
    TEST(PatchStream, StringView) {
        std::stringstream ss;
        ss << u8"hello"sv;
        EXPECT_EQ(REINTERPRET::as_utf8_view(ss.view()), u8"hello"sv);
    }

    TEST(PatchStream, CStrPtr) {
        std::stringstream ss;
        ss << u8"hello";
        EXPECT_EQ(REINTERPRET::as_utf8_view(ss.view()), u8"hello");
    }

    TEST(PatchStream, Character) {
        std::stringstream ss;
        ss << u8'y';
        EXPECT_EQ(REINTERPRET::as_utf8_view(ss.view()), u8"y");
    }

}
