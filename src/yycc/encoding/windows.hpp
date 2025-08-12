#pragma once
#include "../macro/os_detector.hpp"
#include "../macro/stl_detector.hpp"
#include <string>
#include <string_view>
#include <expected>
#include <cstdint>

namespace yycc::encoding::windows {

    /// @brief The type of Windows code page.
    using CodePage = uint32_t;

    /// @brief The possible error kind occurs in this module.
    enum class ConvError {
        TooLargeLength, ///< The length of given string is too large exceeding the maximum capacity of Win32 function.
        NoDesiredSize,  ///< Can not compute the desired size of result string.
        BadWrittenSize, ///< The size of written data is not matched with expected size.
        InvalidUtf32,   ///< Given char is invalid in UTF32.
        InvalidUtf16,   ///< Given char is invalid in UTF16.
        EncodeUtf8,     ///< Error occurs when encoding UTF8.
        IncompleteUtf8, ///< Given UTF8 string is incomplete.
    };

    /// @brief The result type in this module.
    template<typename T>
    using ConvResult = std::expected<T, ConvError>;

#if defined(YYCC_OS_WINDOWS)

    /**
     * @brief WChar -> Char
     * @param[in] src The string to be converted.
     * @param[in] code_page The code page of native string.
     * @return The converted string, or error occurring.
     */
    ConvResult<std::string> to_char(const std::wstring_view& src, CodePage code_page);

    /**
     * @brief Char -> WChar
     * @param[in] src The string to be converted.
     * @param[in] code_page The code page of native string.
     * @return The converted string, or error occurring.
     */
    ConvResult<std::wstring> to_wchar(const std::string_view& src, CodePage code_page);

    /**
     * @brief Char -> Char
     * @details This is the combination of "WChar -> Char" and "Char -> WChar"
     * @param[in] src The string to be converted.
     * @param[in] src_code_page The code page of source string.
     * @param[in] dst_code_page The code page of destination string.
     * @return The converted string, or error occurring.
     */
    ConvResult<std::string> to_char(const std::string_view& src, CodePage src_code_page, CodePage dst_code_page);

    /**
     * @brief WChar -> UTF8
     * @details This is the specialization of "WChar -> Char"
     * @param[in] src The string to be converted.
     * @return The converted string, or error occurring.
     */
    ConvResult<std::u8string> to_utf8(const std::wstring_view& src);

    /**
     * @brief UTF8 -> WChar
     * @details This is the specialization of "Char -> WChar"
     * @param[in] src The string to be converted.
     * @return The converted string, or error occurring.
     */
    ConvResult<std::wstring> to_wchar(const std::u8string_view& src);

    /**
     * @brief Char -> UTF8
     * @details This is the specialization of "Char -> Char"
     * @param[in] src The string to be converted.
     * @param[in] code_page The code page of native string.
     * @return The converted string, or error occurring.
     */
    ConvResult<std::u8string> to_utf8(const std::string_view& src, CodePage code_page);

    /**
     * @brief UTF8 -> Char
     * @details This is the specialization of "Char -> Char"
     * @param[in] src The string to be converted.
     * @param[in] code_page The code page of native string.
     * @return The converted string, or error occurring.
     */
    ConvResult<std::string> to_char(const std::u8string_view& src, CodePage code_page);

    // YYC MARK:
    // UTF convertion only works on Microsoft STL.
    // See implementation for more details

#if defined(YYCC_STL_MSSTL)

    /**
     * @brief UTF8 -> UTF16
     * @param[in] src The string to be converted.
     * @return The converted string, or error occurring.
     */
    ConvResult<std::u16string> to_utf16(const std::u8string_view& src);

    /**
     * @brief UTF16 -> UTF8
     * @param[in] src The string to be converted.
     * @return The converted string, or error occurring.
     */
    ConvResult<std::u8string> to_utf8(const std::u16string_view& src);

    /**
     * @brief UTF8 -> UTF32
     * @param[in] src The string to be converted.
     * @return The converted string, or error occurring.
     */
    ConvResult<std::u32string> to_utf32(const std::u8string_view& src);

    /**
     * @brief UTF32 -> UTF8
     * @param[in] src The string to be converted.
     * @return The converted string, or error occurring.
     */
    ConvResult<std::u8string> to_utf8(const std::u32string_view& src);

#endif

#endif

} // namespace yycc::encoding::windows
