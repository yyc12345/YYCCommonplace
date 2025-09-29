#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/encoding/windows.hpp>
#include "../../shared/literals.hpp"

#define ENC ::yycc::encoding::windows

namespace yycctest::encoding::windows {
#if defined(YYCC_OS_WINDOWS)

    static auto UTF_LITERALS = ::yyccshared::literals::UtfLiterals();
    static auto OTHER_LITERALS = ::yyccshared::literals::OtherLiterals();

    TEST(EncodingWindows, CharToWchar) {
        const auto& other_str_literals = OTHER_LITERALS.get_other_str_vec();
        const auto& wstr_literals = OTHER_LITERALS.get_wstr_vec();

        for (size_t i = 0; i < OTHER_LITERALS.get_size(); ++i) {
            const auto& other_str_literal = other_str_literals[i];

            auto rv = ENC::to_wchar(other_str_literal.get_other_str(), other_str_literal.get_windows_ident());
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), wstr_literals[i]);
        }
    }

    TEST(EncodingWindows, WcharToChar) {
        const auto& other_str_literals = OTHER_LITERALS.get_other_str_vec();
        const auto& wstr_literals = OTHER_LITERALS.get_wstr_vec();

        for (size_t i = 0; i < OTHER_LITERALS.get_size(); ++i) {
            const auto& other_str_literal = other_str_literals[i];

            auto rv = ENC::to_char(wstr_literals[i], other_str_literal.get_windows_ident());
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), other_str_literal.get_other_str());
        }
    }

    TEST(EncodingWindows, CharToUtf8) {
        const auto& other_str_literals = OTHER_LITERALS.get_other_str_vec();
        const auto& u8str_literals = OTHER_LITERALS.get_u8str_vec();

        for (size_t i = 0; i < OTHER_LITERALS.get_size(); ++i) {
            const auto& other_str_literal = other_str_literals[i];

            auto rv = ENC::to_utf8(other_str_literal.get_other_str(), other_str_literal.get_windows_ident());
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), u8str_literals[i]);
        }
    }

    TEST(EncodingWindows, Utf8ToChar) {
        const auto& other_str_literals = OTHER_LITERALS.get_other_str_vec();
        const auto& u8str_literals = OTHER_LITERALS.get_u8str_vec();

        for (size_t i = 0; i < OTHER_LITERALS.get_size(); ++i) {
            const auto& other_str_literal = other_str_literals[i];

            auto rv = ENC::to_char(u8str_literals[i], other_str_literal.get_windows_ident());
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), other_str_literal.get_other_str());
        }
    }

    TEST(EncodingWindows, Utf8ToWchar) {
        const auto& u8str_literals = UTF_LITERALS.get_u8str_vec();
        const auto& wstr_literals = UTF_LITERALS.get_wstr_vec();

        for (size_t i = 0; i < UTF_LITERALS.get_size(); ++i) {
            auto rv = ENC::to_wchar(u8str_literals[i]);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), wstr_literals[i]);
        }
    }

    TEST(EncodingWindows, WcharToUtf8) {
        const auto& u8str_literals = UTF_LITERALS.get_u8str_vec();
        const auto& wstr_literals = UTF_LITERALS.get_wstr_vec();

        for (size_t i = 0; i < UTF_LITERALS.get_size(); ++i) {
            auto rv = ENC::to_utf8(wstr_literals[i]);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), u8str_literals[i]);
        }
    }

#if defined(YYCC_STL_MSSTL)

    TEST(EncodingWindows, Utf8ToUtf16) {
        const auto& u8str_literals = UTF_LITERALS.get_u8str_vec();
        const auto& u16str_literals = UTF_LITERALS.get_u16str_vec();

        for (size_t i = 0; i < UTF_LITERALS.get_size(); ++i) {
            auto rv = ENC::to_utf16(u8str_literals[i]);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), u16str_literals[i]);
        }
    }

    TEST(EncodingWindows, Utf16ToUtf8) {
        const auto& u8str_literals = UTF_LITERALS.get_u8str_vec();
        const auto& u16str_literals = UTF_LITERALS.get_u16str_vec();

        for (size_t i = 0; i < UTF_LITERALS.get_size(); ++i) {
            auto rv = ENC::to_utf8(u16str_literals[i]);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), u8str_literals[i]);
        }
    }

    TEST(EncodingWindows, Utf8ToUtf32) {
        const auto& u8str_literals = UTF_LITERALS.get_u8str_vec();
        const auto& u32str_literals = UTF_LITERALS.get_u32str_vec();

        for (size_t i = 0; i < UTF_LITERALS.get_size(); ++i) {
            auto rv = ENC::to_utf32(u8str_literals[i]);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), u32str_literals[i]);
        }
    }

    TEST(EncodingWindows, Utf32ToUtf8) {
        const auto& u8str_literals = UTF_LITERALS.get_u8str_vec();
        const auto& u32str_literals = UTF_LITERALS.get_u32str_vec();

        for (size_t i = 0; i < UTF_LITERALS.get_size(); ++i) {
            auto rv = ENC::to_utf8(u32str_literals[i]);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), u8str_literals[i]);
        }
    }

#endif

#endif
} // namespace yycctest::encoding::windows
