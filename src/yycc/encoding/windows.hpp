#pragma once
#include "../macro/os_detector.hpp"

#if defined(YYCC_OS_WINDOWS)

#include "../patch/expected.hpp"
#include "../string.hpp"
#include <cstdint>

#define NS_YYCC_STRING ::yycc::string
#define NS_YYCC_PATCH_EXPECTED ::yycc::patch::expected

namespace yycc::encoding::windows {

    using CodePage = uint32_t;

    /// @private
    enum class ConvError {
        TooLargeLength, ///< The length of given string is too large exceeding the maximum capacity of Win32 function.
        NoDesiredSize,  ///< Can not compute the desired size of result string.
        BadWrittenSize, ///< The size of written data is not matched with expected size.
        InvalidUtf32,   ///< Given char is invalid in UTF32.
        InvalidUtf16,   ///< Given char is invalid in UTF16.
        EncodeUtf8,     ///< Error occurs when encoding UTF8.
        IncompleteUtf8, ///< Given UTF8 string is incomplete.
    };

    /// @private
    template<typename T>
    using ConvResult = NS_YYCC_PATCH_EXPECTED::Expected<T, ConvError>;

    // WChar -> Char
    ConvResult<std::string> priv_to_char(const std::wstring_view& src, CodePage code_page);
    bool to_char(const std::wstring_view& src, std::string& dst, CodePage code_page);
    std::string to_char(const std::wstring_view& src, CodePage code_page);

    // Char -> WChar
    ConvResult<std::wstring> priv_to_wchar(const std::string_view& src, CodePage code_page);
    bool to_wchar(const std::string_view& src, std::wstring& dst, CodePage code_page);
    std::wstring to_wchar(const std::string_view& src, CodePage code_page);

    // Char -> Char
    // This is the combination of "WChar -> Char" and "Char -> WChar"
    ConvResult<std::string> priv_to_char(const std::string_view& src, CodePage src_code_page, CodePage dst_code_page);
    bool to_char(const std::string_view& src, std::string& dst, CodePage src_code_page, CodePage dst_code_page);
    std::string to_char(const std::string_view& src, CodePage src_code_page, CodePage dst_code_page);

    // WChar -> UTF8
    // This is the specialization of "WChar -> Char"
    ConvResult<NS_YYCC_STRING::u8string> priv_to_utf8(const std::wstring_view& src);
    bool to_utf8(const std::wstring_view& src, NS_YYCC_STRING::u8string& dst);
    NS_YYCC_STRING::u8string to_utf8(const std::wstring_view& src);

    // UTF8 -> WChar
    // This is the specialization of "Char -> WChar"
    ConvResult<std::wstring> priv_to_wchar(const NS_YYCC_STRING::u8string_view& src);
    bool to_wchar(const NS_YYCC_STRING::u8string_view& src, std::wstring& dst);
    std::wstring to_wchar(const NS_YYCC_STRING::u8string_view& src);

    // Char -> UTF8
    // This is the specialization of "Char -> Char"
    ConvResult<NS_YYCC_STRING::u8string> priv_to_utf8(const std::string_view& src, CodePage code_page);
    bool to_utf8(const std::string_view& src, NS_YYCC_STRING::u8string& dst, CodePage code_page);
    NS_YYCC_STRING::u8string to_utf8(const std::string_view& src, CodePage code_page);

    // UTF8 -> Char
    // This is the specialization of "Char -> Char"
    ConvResult<std::string> priv_to_char(const NS_YYCC_STRING::u8string_view& src, CodePage code_page);
    bool to_char(const NS_YYCC_STRING::u8string_view& src, std::string& dst, CodePage code_page);
    std::string to_char(const NS_YYCC_STRING::u8string_view& src, CodePage code_page);

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

} // namespace yycc::encoding::windows

#undef NS_YYCC_PATCH_EXPECTED
#undef NS_YYCC_STRING

#endif
