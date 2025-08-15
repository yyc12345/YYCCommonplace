#pragma once
#include <string_view>
#include <expected>

/**
 * @brief The namespace replicating Linux-specialized function, "wcswidth", in all platforms.
 * @details
 * "wcswdith" is a specialized function in Linux.
 * It was not included in POSIX standard and only provided on Linux.
 * This function can fetch how many space which given string occupied in terminal.
 * This is essential and useful function in our library.
 * So I create this namespace to make "wcswidth" be available on all platforms.
 *
 * "wcswidth" is based on \c wchar_t. In Linux, \c wchar_t is 4-bytes length.
 * It can represent any characters without surrogate pair.
 * However, in Windows, \c wchar_t is 2-bytes length.
 * There is possible surrogate pair within \c wchar_t string, which is inconvenient for our programming.
 * So in this homebrew namespace, I forcelt use \c char32_t as the basic char type.
 *
 * Due to the requirements of mine, this implementation is slightly different with original one.
 * These differences are list below:
 *
 * \li We do not return negative value for Control Char in "wcwidth",
 * because we need to support the analyse of ANSI Escape Sequence.
 * \li Due to the previous change, the type of return value of "wcwidth" and "wcswidth"
 * are changed from \c int to \c size_t because there is no negative return value.
 * \li "wcswidth" now support ANSI Escape Sequence (e.g. terminal color).
 * So it can analyse colorful output with correct space.
 */
namespace yycc::carton::wcwidth {

    /// @brief Error occurs in this module
    enum class Error {
        BadEncoding,   ///< Given
        BadAnsiEscSeq, ///< Bad char when processing ANSI Escape Sequence
        BadCsiSeq,     ///< Bad char when processing CSI Sequence.
    };

    /// @brief Result type for this module
    template<typename T>
    using Result = std::expected<T, Error>;

    size_t wcwidth(char32_t wc);
    Result<size_t> wcswidth(const std::u32string_view& rhs);
    Result<size_t> wcswidth(const std::u8string_view& rhs);

} // namespace yycc::carton::wcwidth
