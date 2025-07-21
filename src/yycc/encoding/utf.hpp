#pragma once
#include "../string.hpp"
#include "../patch/expected.hpp"

#define NS_YYCC_STRING ::yycc::string
#define NS_YYCC_PATCH_EXPECTED ::yycc::patch::expected

namespace yycc::encoding::utf {

    /// @private
    struct ConvError {};

    /// @private
    template<typename T>
    using ConvResult = NS_YYCC_PATCH_EXPECTED::Expected<T, ConvError>;

    // UTF8 -> UTF16
    ConvResult<std::u16string> priv_to_utf16(const NS_YYCC_STRING::u8string_view& src);
    bool to_utf16(const NS_YYCC_STRING::u8string_view& src, std::u16string& dst);
    std::u16string to_utf16(const NS_YYCC_STRING::u8string_view& src);

    // UTF16 -> UTF8
    ConvResult<NS_YYCC_STRING::u8string> priv_to_utf8(const std::u16string_view& src);
    bool to_utf8(const std::u16string_view& src, NS_YYCC_STRING::u8string& dst);
    NS_YYCC_STRING::u8string to_utf8(const std::u16string_view& src);

    // UTF8 -> UTF32
    ConvResult<std::u32string> priv_to_utf32(const NS_YYCC_STRING::u8string_view& src);
    bool to_utf32(const NS_YYCC_STRING::u8string_view& src, std::u32string& dst);
    std::u32string to_utf32(const NS_YYCC_STRING::u8string_view& src);

    // UTF32 -> UTF8
    ConvResult<NS_YYCC_STRING::u8string> priv_to_utf8(const std::u32string_view& src);
    bool to_utf8(const std::u32string_view& src, NS_YYCC_STRING::u8string& dst);
    NS_YYCC_STRING::u8string to_utf8(const std::u32string_view& src);

}

#undef NS_YYCC_PATCH_EXPECTED
#undef NS_YYCC_STRING
