#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/encoding/stl.hpp>
#include "../../shared/literals.hpp"

#define ENC ::yycc::encoding::stl

namespace yycctest::encoding::stl {

    static auto UTF_LITERALS = ::yyccshared::literals::UtfLiterals();

    TEST(EncodingStl, Utf8ToUtf16) {
        const auto& u8str_literals = UTF_LITERALS.get_u8str_vec();
        const auto& u16str_literals = UTF_LITERALS.get_u16str_vec();

        for (size_t i = 0; i < UTF_LITERALS.get_size(); ++i) {
            auto rv = ENC::to_utf16(u8str_literals[i]);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), u16str_literals[i]);
        }
    }

    TEST(EncodingStl, Utf16ToUtf8) {
        const auto& u8str_literals = UTF_LITERALS.get_u8str_vec();
        const auto& u16str_literals = UTF_LITERALS.get_u16str_vec();

        for (size_t i = 0; i < UTF_LITERALS.get_size(); ++i) {
            auto rv = ENC::to_utf8(u16str_literals[i]);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), u8str_literals[i]);
        }
    }

    TEST(EncodingStl, Utf8ToUtf32) {
        const auto& u8str_literals = UTF_LITERALS.get_u8str_vec();
        const auto& u32str_literals = UTF_LITERALS.get_u32str_vec();

        for (size_t i = 0; i < UTF_LITERALS.get_size(); ++i) {
            auto rv = ENC::to_utf32(u8str_literals[i]);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), u32str_literals[i]);
        }
    }

    TEST(EncodingStl, Utf32ToUtf8) {
        const auto& u8str_literals = UTF_LITERALS.get_u8str_vec();
        const auto& u32str_literals = UTF_LITERALS.get_u32str_vec();

        for (size_t i = 0; i < UTF_LITERALS.get_size(); ++i) {
            auto rv = ENC::to_utf8(u32str_literals[i]);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), u8str_literals[i]);
        }
    }

} // namespace yycctest::encoding::stl
