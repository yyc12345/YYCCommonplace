#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/carton/pycodec.hpp>
#include "../../shared/literals.hpp"

#define ENC ::yycc::carton::pycodec

namespace yycctest::carton::pycodec {

    static auto UTF_LITERALS = ::yyccshared::literals::UtfLiterals();
    static auto OTHER_LITERALS = ::yyccshared::literals::OtherLiterals();

    TEST(CartonPycodec, ValidateName) {
        EXPECT_TRUE(ENC::is_valid_encoding_name(u8"utf-8"));
        EXPECT_TRUE(ENC::is_valid_encoding_name(u8"gb2312"));
        EXPECT_TRUE(ENC::is_valid_encoding_name(u8"cp1252"));

        EXPECT_FALSE(ENC::is_valid_encoding_name(u8"this must not be a valid encoding name"));
    }

    TEST(CartonPycodec, CharToUtf8) {
        const auto& other_str_literals = OTHER_LITERALS.get_other_str_vec();
        const auto& u8str_literals = OTHER_LITERALS.get_u8str_vec();

        for (size_t i = 0; i < OTHER_LITERALS.get_size(); ++i) {
            const auto& other_str_literal = other_str_literals[i];
            ENC::CharToUtf8 cv(other_str_literal.get_pycodec_ident());

            auto rv = cv.to_utf8(other_str_literal.get_other_str());
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), u8str_literals[i]);
        }
    }

    TEST(CartonPycodec, Utf8ToChar) {
        const auto& other_str_literals = OTHER_LITERALS.get_other_str_vec();
        const auto& u8str_literals = OTHER_LITERALS.get_u8str_vec();

        for (size_t i = 0; i < OTHER_LITERALS.get_size(); ++i) {
            const auto& other_str_literal = other_str_literals[i];
            ENC::Utf8ToChar cv(other_str_literal.get_pycodec_ident());

            auto rv = cv.to_char(u8str_literals[i]);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), other_str_literal.get_other_str());
        }
    }

    TEST(CartonPycodec, Utf8ToWchar) {
        const auto& u8str_literals = UTF_LITERALS.get_u8str_vec();
        const auto& wstr_literals = UTF_LITERALS.get_wstr_vec();

        ENC::Utf8ToWchar cv;
        for (size_t i = 0; i < UTF_LITERALS.get_size(); ++i) {
            auto rv = cv.to_wchar(u8str_literals[i]);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), wstr_literals[i]);
        }
    }

    TEST(CartonPycodec, WcharToUtf8) {
        const auto& u8str_literals = UTF_LITERALS.get_u8str_vec();
        const auto& wstr_literals = UTF_LITERALS.get_wstr_vec();

        ENC::WcharToUtf8 cv;
        for (size_t i = 0; i < UTF_LITERALS.get_size(); ++i) {
            auto rv = cv.to_utf8(wstr_literals[i]);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), u8str_literals[i]);
        }
    }

    TEST(CartonPycodec, Utf8ToUtf16) {
        const auto& u8str_literals = UTF_LITERALS.get_u8str_vec();
        const auto& u16str_literals = UTF_LITERALS.get_u16str_vec();

        ENC::Utf8ToUtf16 cv;
        for (size_t i = 0; i < UTF_LITERALS.get_size(); ++i) {
            auto rv = cv.to_utf16(u8str_literals[i]);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), u16str_literals[i]);
        }
    }

    TEST(CartonPycodec, Utf16ToUtf8) {
        const auto& u8str_literals = UTF_LITERALS.get_u8str_vec();
        const auto& u16str_literals = UTF_LITERALS.get_u16str_vec();

        ENC::Utf16ToUtf8 cv;
        for (size_t i = 0; i < UTF_LITERALS.get_size(); ++i) {
            auto rv = cv.to_utf8(u16str_literals[i]);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), u8str_literals[i]);
        }
    }

    TEST(CartonPycodec, Utf8ToUtf32) {
        const auto& u8str_literals = UTF_LITERALS.get_u8str_vec();
        const auto& u32str_literals = UTF_LITERALS.get_u32str_vec();

        ENC::Utf8ToUtf32 cv;
        for (size_t i = 0; i < UTF_LITERALS.get_size(); ++i) {
            auto rv = cv.to_utf32(u8str_literals[i]);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), u32str_literals[i]);
        }
    }

    TEST(CartonPycodec, Utf32ToUtf8) {
        const auto& u8str_literals = UTF_LITERALS.get_u8str_vec();
        const auto& u32str_literals = UTF_LITERALS.get_u32str_vec();

        ENC::Utf32ToUtf8 cv;
        for (size_t i = 0; i < UTF_LITERALS.get_size(); ++i) {
            auto rv = cv.to_utf8(u32str_literals[i]);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), u8str_literals[i]);
        }
    }

}
