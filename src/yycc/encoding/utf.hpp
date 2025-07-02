#pragma once
#include <yycc/string.hpp>
#include <type_traits>
#include <variant>

#define NS_YYCC_STRING ::yycc::string

namespace yycc::encoding::utf {

    /// @private
    struct ConvError {};

    /// @private
    template<typename T, std::enable_if_t<!std::is_same_v<T, ConvError>, int> = 0>
    using ConvResult = std::variant<T, ConvError>;

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

#undef NS_YYCC_STRING
