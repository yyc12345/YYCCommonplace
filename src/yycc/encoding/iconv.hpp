#pragma once
#include "../macro/os_detector.hpp"
#include "../macro/class_copy_move.hpp"
#include <string>
#include <string_view>
#include <expected>
#include <memory>

namespace yycc::encoding::iconv {

    // YYC MARK:
    // I don't want to include "iconv.h" in there.
    // One of reasons is that I want to hide all implementation of Iconv.
    // Another reason is that "iconv.h" defines some annoying macros which intervene some names in this files.
    // So I introduce PIMPL design mode. Use a pointer to hide all details in class PrivToken.

    /// @brief The code name type used by Iconv.
    using CodeName = std::string_view;

    /// @private
    class PrivToken;

    /// @private
    class Token {
    public:
        Token(const CodeName& from_code, const CodeName& to_code);
        ~Token();
        YYCC_DELETE_COPY(Token)
        YYCC_DEFAULT_MOVE(Token)

        bool is_valid() const;
        PrivToken* get_inner() const;

    private:
        std::unique_ptr<PrivToken> inner;
    };

    /// @brief The possible error occurs in this module.
    enum class ConvError {
        InvalidCd,       ///< Given token is invalid.
        NullPointer,     ///< Some of essential pointer in argument is nullptr.
        InvalidMbSeq,    ///< An invalid multibyte sequence has been encountered in the input.
        IncompleteMbSeq, ///< An incomplete multibyte sequence has been encountered in the input.
        BadRv,           ///< The size of encoding convertion is not matched with expected char type.
    };

    /// @brief The result type in this module.
    template<typename T>
    using ConvResult = std::expected<T, ConvError>;

#if YYCC_FEAT_ICONV || !defined(YYCC_OS_WINDOWS)

    /// @brief Char -> UTF8
    class CharToUtf8 {
    public:
        CharToUtf8(const CodeName& code_name);
        ~CharToUtf8();
        YYCC_DELETE_COPY(CharToUtf8)
        YYCC_DEFAULT_MOVE(CharToUtf8)

    public:
        ConvResult<std::u8string> to_utf8(const std::string_view& src);

    private:
        Token token;
    };

    /// @brief UTF8 -> Char
    class Utf8ToChar {
    public:
        Utf8ToChar(const CodeName& code_name);
        ~Utf8ToChar();
        YYCC_DELETE_COPY(Utf8ToChar)
        YYCC_DEFAULT_MOVE(Utf8ToChar)

    public:
        ConvResult<std::string> to_char(const std::u8string_view& src);

    private:
        Token token;
    };

    /// @brief WChar -> UTF8
    class WcharToUtf8 {
    public:
        WcharToUtf8();
        ~WcharToUtf8();
        YYCC_DELETE_COPY(WcharToUtf8)
        YYCC_DEFAULT_MOVE(WcharToUtf8)

    public:
        ConvResult<std::u8string> to_utf8(const std::wstring_view& src);

    private:
        Token token;
    };

    /// @brief UTF8 -> WChar
    class Utf8ToWchar {
    public:
        Utf8ToWchar();
        ~Utf8ToWchar();
        YYCC_DELETE_COPY(Utf8ToWchar)
        YYCC_DEFAULT_MOVE(Utf8ToWchar)

    public:
        ConvResult<std::wstring> to_wchar(const std::u8string_view& src);

    private:
        Token token;
    };

    /// @brief UTF8 -> UTF16
    class Utf8ToUtf16 {
    public:
        Utf8ToUtf16();
        ~Utf8ToUtf16();
        YYCC_DELETE_COPY(Utf8ToUtf16)
        YYCC_DEFAULT_MOVE(Utf8ToUtf16)

    public:
        ConvResult<std::u16string> to_utf16(const std::u8string_view& src);

    private:
        Token token;
    };

    /// @brief UTF16 -> UTF8
    class Utf16ToUtf8 {
    public:
        Utf16ToUtf8();
        ~Utf16ToUtf8();
        YYCC_DELETE_COPY(Utf16ToUtf8)
        YYCC_DEFAULT_MOVE(Utf16ToUtf8)

    public:
        ConvResult<std::u8string> to_utf8(const std::u16string_view& src);

    private:
        Token token;
    };

    /// @brief UTF8 -> UTF32
    class Utf8ToUtf32 {
    public:
        Utf8ToUtf32();
        ~Utf8ToUtf32();
        YYCC_DELETE_COPY(Utf8ToUtf32)
        YYCC_DEFAULT_MOVE(Utf8ToUtf32)

    public:
        ConvResult<std::u32string> to_utf32(const std::u8string_view& src);

    private:
        Token token;
    };

    /// @brief UTF32 -> UTF8
    class Utf32ToUtf8 {
    public:
        Utf32ToUtf8();
        ~Utf32ToUtf8();
        YYCC_DELETE_COPY(Utf32ToUtf8)
        YYCC_DEFAULT_MOVE(Utf32ToUtf8)

    public:
        ConvResult<std::u8string> to_utf8(const std::u32string_view& src);

    private:
        Token token;
    };

#endif

} // namespace yycc::encoding::iconv
