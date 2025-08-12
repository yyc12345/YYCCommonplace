#pragma once
#include <string>
#include <string_view>
#include <expected>

namespace yycc::encoding::stl {

    /// @brief Possible convertion error occurs in this module.
    struct ConvError {};

    /// @brief The result type of this module.
    template<typename T>
    using ConvResult = std::expected<T, ConvError>;

    /**
     * @brief UTF8 -> UTF16
     * @param src
     * @return
     */
    ConvResult<std::u16string> to_utf16(const std::u8string_view& src);

    /**
     * @brief UTF16 -> UTF8
     * @param src
     * @return
     */
    ConvResult<std::u8string> to_utf8(const std::u16string_view& src);

    /**
     * @brief UTF8 -> UTF32
     * @param src
     * @return
     */
    ConvResult<std::u32string> to_utf32(const std::u8string_view& src);

    /**
     * @brief UTF32 -> UTF8
     * @param src
     * @return
     */
    ConvResult<std::u8string> to_utf8(const std::u32string_view& src);

}
