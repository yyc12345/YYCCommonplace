#include <cstring>
#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/string/reinterpret.hpp>

#include <yycc/prelude/core.hpp>

#define REINTERPRET ::yycc::string::reinterpret
#define CONST_VOID_PTR(p) reinterpret_cast<const void*>(p)
#define VOID_PTR(p) reinterpret_cast<void*>(p)

namespace yycctest::string::reinterpret {

    static u8string PROBE(YYCC_U8("Test"));

    TEST(StringReinterpret, ConstPointer) {
        const auto* src = PROBE.data();
        const auto* dst = REINTERPRET::as_ordinary(src);
        const auto* new_src = REINTERPRET::as_utf8(dst);

        // Pointer should point to the same address after casting.
        EXPECT_EQ(CONST_VOID_PTR(src), CONST_VOID_PTR(dst));
        EXPECT_EQ(CONST_VOID_PTR(src), CONST_VOID_PTR(new_src));
    }

    TEST(StringReinterpret, Pointer) {
        auto* src = PROBE.data();
        auto* dst = REINTERPRET::as_ordinary(src);
        auto* new_src = REINTERPRET::as_utf8(dst);

        // Pointer should point to the same address after casting.
        EXPECT_EQ(VOID_PTR(src), VOID_PTR(dst));
        EXPECT_EQ(VOID_PTR(src), VOID_PTR(new_src));
    }

    TEST(StringReinterpret, String) {
        auto src = u8string(PROBE);
        auto dst = REINTERPRET::as_ordinary(src);
        auto new_src = REINTERPRET::as_utf8(dst);

        // Check memory length and data.
        ASSERT_EQ(src.length(), dst.length());
        EXPECT_TRUE(std::memcmp(src.data(), dst.data(), src.length()) == 0);
        ASSERT_EQ(src.length(), new_src.length());
        EXPECT_TRUE(std::memcmp(src.data(), new_src.data(), src.length()) == 0);
    }

    TEST(StringReinterpret, StringView) {
        auto src = u8string_view(PROBE);
        auto dst = REINTERPRET::as_ordinary_view(src);
        auto new_src = REINTERPRET::as_utf8_view(dst);

        // Check memory length and data.
        ASSERT_EQ(src.length(), dst.length());
        EXPECT_TRUE(std::memcmp(src.data(), dst.data(), src.length()) == 0);
        ASSERT_EQ(src.length(), new_src.length());
        EXPECT_TRUE(std::memcmp(src.data(), new_src.data(), src.length()) == 0);
    }

} // namespace yycctest::string::reinterpret
