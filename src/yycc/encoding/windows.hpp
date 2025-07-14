#pragma once
#include "../macro/os_detector.hpp"
#include "../string.hpp"
#include "../patch/expected.hpp"
#include <cstdint>

#define NS_YYCC_STRING ::yycc::string
#define NS_YYCC_PATCH_EXPECTED ::yycc::patch::expected

namespace yycc::encoding::windows {

#if YYCC_OS == YYCC_OS_WINDOWS

    using CodePage = uint32_t;

    /// @private
    enum class ConvError {
        TooLargeLength, ///< The length of given string is too large exceeding the maximum capacity of Win32 function.
        NoDesiredSize, ///< Can not compute the desired size of result string.
        BadWrittenSize, ///< The size of written data is not matched with expected size.
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

    // YYC MARK:
    // Following functions are basically the alias of above functions.

    // Char -> Char
    ConvResult<std::string> priv_to_char(const std::string_view& src,
                                         CodePage src_code_page,
                                         CodePage dst_code_page);
    bool to_char(const std::string_view& src,
                 std::string& dst,
                 CodePage src_code_page,
                 CodePage dst_code_page);
    std::string to_char(const std::string_view& src, CodePage src_code_page, CodePage dst_code_page);

    // YYC MARK:
    // Following functions are basically the specialized UTF8 functions.

    // WChar -> UTF8
    ConvResult<NS_YYCC_STRING::u8string> priv_to_utf8(const std::wstring_view& src);
    bool to_utf8(const std::wstring_view& src, NS_YYCC_STRING::u8string& dst);
    NS_YYCC_STRING::u8string to_utf8(const std::wstring_view& src);

    // UTF8 -> WChar
    ConvResult<std::wstring> priv_to_wchar(const NS_YYCC_STRING::u8string_view& src);
    bool to_wchar(const NS_YYCC_STRING::u8string_view& src, std::wstring& dst);
    std::wstring to_wchar(const NS_YYCC_STRING::u8string_view& src);

    // Char -> UTF8
    ConvResult<NS_YYCC_STRING::u8string> priv_to_utf8(const std::string_view& src,
                                                      CodePage code_page);
    bool to_utf8(const std::string_view& src, NS_YYCC_STRING::u8string& dst, CodePage code_page);
    NS_YYCC_STRING::u8string to_utf8(const std::string_view& src, CodePage code_page);

    // UTF8 -> Char
    ConvResult<std::string> priv_to_char(const NS_YYCC_STRING::u8string_view& src,
                                         CodePage code_page);
    bool to_char(const NS_YYCC_STRING::u8string_view& src, std::string& dst, CodePage code_page);
    std::string to_char(const NS_YYCC_STRING::u8string_view& src, CodePage code_page);

#endif

} // namespace yycc::encoding::windows

#undef NS_YYCC_PATCH_EXPECTED
#undef NS_YYCC_STRING
