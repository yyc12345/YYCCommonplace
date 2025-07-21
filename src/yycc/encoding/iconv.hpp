#pragma once
#include "../macro/os_detector.hpp"

#if YYCC_FEAT_ICONV || (YYCC_OS != YYCC_OS_WINDOWS)

#include "../macro/class_copy_move.hpp"
#include "../patch/expected.hpp"
#include "../string.hpp"
#include <memory>

#define NS_YYCC_STRING ::yycc::string
#define NS_YYCC_PATCH_EXPECTED ::yycc::patch::expected

namespace yycc::encoding::iconv {

    // YYC MARK:
    // I don't want to include "iconv.h" in there.
    // One of reasons is that I want to hide all implementation of Iconv.
    // Another reason is that "iconv.h" defines some annoying macros which intervene some names in this files.
    // So I introduce PIMPL design mode. Use a pointer to hide all details in class PrivToken.

    using CodeName = NS_YYCC_STRING::u8string_view;

    /// @private
    class PrivToken;

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

    /// @private
    enum class ConvError {
        InvalidCd,       ///< Given token is invalid.
        NullPointer,     ///< Some of essential pointer in argument is nullptr.
        InvalidMbSeq,    ///< An invalid multibyte sequence has been encountered in the input.
        IncompleteMbSeq, ///< An incomplete multibyte sequence has been encountered in the input.
    };

    /// @private
    template<typename T>
    using ConvResult = NS_YYCC_PATCH_EXPECTED::Expected<T, ConvError>;

    // TODO: 为下列类添加注释时，指明static开头的是一次性使用的，适用于转换一两次结束。
    // 用实例类的适用于需要持续转换的。

    // Char -> UTF8
    class CharToUtf8 {
    public:
        CharToUtf8(const CodeName& code_name);
        ~CharToUtf8();
        YYCC_DELETE_COPY(CharToUtf8)
        YYCC_DEFAULT_MOVE(CharToUtf8)

    public:
        ConvResult<NS_YYCC_STRING::u8string> priv_to_utf8(const std::string_view& src);
        bool to_utf8(const std::string_view& src, NS_YYCC_STRING::u8string& dst);
        NS_YYCC_STRING::u8string to_utf8(const std::string_view& src);

    private:
        Token token;
    };

    // UTF8 -> Char
    class Utf8ToChar {
    public:
        Utf8ToChar(const CodeName& code_name);
        ~Utf8ToChar();
        YYCC_DELETE_COPY(Utf8ToChar)
        YYCC_DEFAULT_MOVE(Utf8ToChar)

    public:
        ConvResult<std::string> priv_to_char(const NS_YYCC_STRING::u8string_view& src);
        bool to_char(const NS_YYCC_STRING::u8string_view& src, std::string& dst);
        std::string to_char(const NS_YYCC_STRING::u8string_view& src);

    private:
        Token token;
    };

    // WChar -> Char
    class WcharToChar {
    public:
        WcharToChar(const CodeName& code_name);
        ~WcharToChar();
        YYCC_DELETE_COPY(WcharToChar)
        YYCC_DEFAULT_MOVE(WcharToChar)

    public:
        ConvResult<std::string> priv_to_char(const std::wstring_view& src);
        bool to_char(const std::wstring_view& src, std::string& dst);
        std::string to_char(const std::wstring_view& src);

    private:
        Token token;
    };

    // Char -> WChar
    class CharToWchar {
    public:
        CharToWchar(const CodeName& code_name);
        ~CharToWchar();
        YYCC_DELETE_COPY(CharToWchar)
        YYCC_DEFAULT_MOVE(CharToWchar)

    public:
        ConvResult<std::wstring> priv_to_wchar(const std::string_view& src, const CodeName& code_name);
        bool to_wchar(const std::string_view& src, std::wstring& dst, const CodeName& code_name);
        std::wstring to_wchar(const std::string_view& src, const CodeName& code_name);

    private:
        Token token;
    };

    // UTF8 -> UTF16
    class Utf8ToUtf16 {
    public:
        Utf8ToUtf16();
        ~Utf8ToUtf16();
        YYCC_DELETE_COPY(Utf8ToUtf16)
        YYCC_DEFAULT_MOVE(Utf8ToUtf16)

    public:
        ConvResult<std::u16string> priv_to_utf16(const NS_YYCC_STRING::u8string_view& src);
        bool to_utf16(const NS_YYCC_STRING::u8string_view& src, std::u16string& dst);
        std::u16string to_utf16(const NS_YYCC_STRING::u8string_view& src);

    private:
        Token token;
    };

    // UTF16 -> UTF8
    class Utf16ToUtf8 {
    public:
        Utf16ToUtf8();
        ~Utf16ToUtf8();
        YYCC_DELETE_COPY(Utf16ToUtf8)
        YYCC_DEFAULT_MOVE(Utf16ToUtf8)

    public:
        ConvResult<NS_YYCC_STRING::u8string> priv_to_utf8(const std::u16string_view& src);
        bool to_utf8(const std::u16string_view& src, NS_YYCC_STRING::u8string& dst);
        NS_YYCC_STRING::u8string to_utf8(const std::u16string_view& src);

    private:
        Token token;
    };

    // UTF8 -> UTF32
    class Utf8ToUtf32 {
    public:
        Utf8ToUtf32();
        ~Utf8ToUtf32();
        YYCC_DELETE_COPY(Utf8ToUtf32)
        YYCC_DEFAULT_MOVE(Utf8ToUtf32)

    public:
        ConvResult<std::u32string> priv_to_utf32(const NS_YYCC_STRING::u8string_view& src);
        bool to_utf32(const NS_YYCC_STRING::u8string_view& src, std::u32string& dst);
        std::u32string to_utf32(const NS_YYCC_STRING::u8string_view& src);

    private:
        Token token;
    };

    // UTF32 -> UTF8
    class Utf32ToUtf8 {
    public:
        Utf32ToUtf8();
        ~Utf32ToUtf8();
        YYCC_DELETE_COPY(Utf32ToUtf8)
        YYCC_DEFAULT_MOVE(Utf32ToUtf8)

    public:
        ConvResult<NS_YYCC_STRING::u8string> priv_to_utf8(const std::u32string_view& src);
        bool to_utf8(const std::u32string_view& src, NS_YYCC_STRING::u8string& dst);
        NS_YYCC_STRING::u8string to_utf8(const std::u32string_view& src);

    private:
        Token token;
    };

} // namespace yycc::encoding::iconv

#undef NS_YYCC_PATCH_EXPECTED
#undef NS_YYCC_STRING

#endif
