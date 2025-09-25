#include <cstring>
#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/string/reinterpret.hpp>

#include <yycc/rust/prelude.hpp>

#define REINTERPRET ::yycc::string::reinterpret
#define AS_UINT8(p) static_cast<u8>(p)
#define CONST_VOID_PTR(p) reinterpret_cast<const void*>(p)
#define VOID_PTR(p) reinterpret_cast<void*>(p)

namespace yycctest::string::reinterpret {

    static std::u8string PROBE(u8"Test");

    TEST(StringReinterpret, Character) {
        const auto& src = PROBE[0];
        const auto dst = REINTERPRET::as_ordinary(src);
        const auto new_src = REINTERPRET::as_utf8(dst);

        // Value should be the same after casting.
        EXPECT_EQ(AS_UINT8(src), AS_UINT8(dst));
        EXPECT_EQ(AS_UINT8(src), AS_UINT8(new_src));
    }

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

    TEST(StringReinterpret, StlString) {
        auto src = std::u8string(PROBE);
        auto dst = REINTERPRET::as_ordinary(src);
        auto new_src = REINTERPRET::as_utf8(dst);

        // Check memory length and data.
        ASSERT_EQ(src.length(), dst.length());
        EXPECT_TRUE(std::memcmp(src.data(), dst.data(), src.length()) == 0);
        ASSERT_EQ(src.length(), new_src.length());
        EXPECT_TRUE(std::memcmp(src.data(), new_src.data(), src.length()) == 0);
    }

    TEST(StringReinterpret, StlStringView) {
        auto src = std::u8string_view(PROBE);
        auto dst = REINTERPRET::as_ordinary_view(src);
        auto new_src = REINTERPRET::as_utf8_view(dst);

        // Check memory length and data.
        ASSERT_EQ(src.length(), dst.length());
        EXPECT_TRUE(std::memcmp(src.data(), dst.data(), src.length()) == 0);
        ASSERT_EQ(src.length(), new_src.length());
        EXPECT_TRUE(std::memcmp(src.data(), new_src.data(), src.length()) == 0);
    }

} // namespace yycctest::string::reinterpret
