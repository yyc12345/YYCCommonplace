#pragma once
#include "../macro/os_detector.hpp"
#include "../macro/class_copy_move.hpp"
#include "../patch/expected.hpp"
#include "../string.hpp"

// Choose the backend of PyCodec module
#if defined(YYCC_OS_WINDOWS)
#include "windows.hpp"
#define YYCC_PYCODEC_WIN32_BACKEND
#define NS_YYCC_ENCODING_BACKEND ::yycc::encoding::windows
#else
#include "iconv.hpp"
#define YYCC_PYCODEC_ICONV_BACKEND
#define NS_YYCC_ENCODING_BACKEND ::yycc::encoding::iconv
#endif

#define NS_YYCC_STRING ::yycc::string
#define NS_YYCC_PATCH_EXPECTED ::yycc::patch::expected

namespace yycc::encoding::pycodec {

    using EncodingName = NS_YYCC_STRING::u8string_view;

    /// @private
    struct ConvError {
        using Error = NS_YYCC_ENCODING_BACKEND::ConvError;
        ConvError(const Error& err);
        Error inner;
    };

    /// @private
    template<typename T>
    using ConvResult = NS_YYCC_PATCH_EXPECTED::Expected<T, ConvError>;

    /**
     * @brief Check whether given name is a valid encoding name in PyCodec.
     * @param[in] name The name to be checked.
     * @return True if it is valid, otherwise false.
     */
    bool is_valid_encoding_name(const EncodingName& name);

    // Char -> UTF8
    class CharToUtf8 {
    public:
        CharToUtf8(const EncodingName& name);
        ~CharToUtf8();
        YYCC_DELETE_COPY(CharToUtf8)
        YYCC_DEFAULT_MOVE(CharToUtf8)

    public:
        ConvResult<NS_YYCC_STRING::u8string> priv_to_utf8(const std::string_view& src);
        bool to_utf8(const std::string_view& src, NS_YYCC_STRING::u8string& dst);
        NS_YYCC_STRING::u8string to_utf8(const std::string_view& src);

    private:
#if defined(YYCC_PYCODEC_WIN32_BACKEND)
        NS_YYCC_ENCODING_BACKEND::CodePage code_page;
#else
        NS_YYCC_ENCODING_BACKEND::CharToUtf8 inner;
#endif
    };

    // UTF8 -> Char
    class Utf8ToChar {
    public:
        Utf8ToChar(const EncodingName& name);
        ~Utf8ToChar();
        YYCC_DELETE_COPY(Utf8ToChar)
        YYCC_DEFAULT_MOVE(Utf8ToChar)

    public:
        ConvResult<std::string> priv_to_char(const NS_YYCC_STRING::u8string_view& src);
        bool to_char(const NS_YYCC_STRING::u8string_view& src, std::string& dst);
        std::string to_char(const NS_YYCC_STRING::u8string_view& src);

    private:
#if defined(YYCC_PYCODEC_WIN32_BACKEND)
        NS_YYCC_ENCODING_BACKEND::CodePage code_page;
#else
        NS_YYCC_ENCODING_BACKEND::Utf8ToChar inner;
#endif
    };

    // WChar -> UTF8
    class WcharToUtf8 {
    public:
        WcharToUtf8();
        ~WcharToUtf8();
        YYCC_DELETE_COPY(WcharToUtf8)
        YYCC_DEFAULT_MOVE(WcharToUtf8)

    public:
        ConvResult<NS_YYCC_STRING::u8string> priv_to_utf8(const std::wstring_view& src);
        bool to_utf8(const std::wstring_view& src, NS_YYCC_STRING::u8string& dst);
        NS_YYCC_STRING::u8string to_utf8(const std::wstring_view& src);

    private:
#if defined(YYCC_PYCODEC_ICONV_BACKEND)
        NS_YYCC_ENCODING_BACKEND::WcharToUtf8 inner;
#endif
    };

    // UTF8 -> WChar
    class Utf8ToWchar {
    public:
        Utf8ToWchar();
        ~Utf8ToWchar();
        YYCC_DELETE_COPY(Utf8ToWchar)
        YYCC_DEFAULT_MOVE(Utf8ToWchar)

    public:
        ConvResult<std::wstring> priv_to_wchar(const NS_YYCC_STRING::u8string_view& src);
        bool to_wchar(const NS_YYCC_STRING::u8string_view& src, std::wstring& dst);
        std::wstring to_wchar(const NS_YYCC_STRING::u8string_view& src);

    private:
#if defined(YYCC_PYCODEC_ICONV_BACKEND)
        NS_YYCC_ENCODING_BACKEND::Utf8ToWchar inner;
#endif
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
#if defined(YYCC_PYCODEC_ICONV_BACKEND)
        NS_YYCC_ENCODING_BACKEND::Utf8ToUtf16 inner;
#endif
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
#if defined(YYCC_PYCODEC_ICONV_BACKEND)
        NS_YYCC_ENCODING_BACKEND::Utf16ToUtf8 inner;
#endif
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
#if defined(YYCC_PYCODEC_ICONV_BACKEND)
        NS_YYCC_ENCODING_BACKEND::Utf8ToUtf32 inner;
#endif
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
#if defined(YYCC_PYCODEC_ICONV_BACKEND)
        NS_YYCC_ENCODING_BACKEND::Utf32ToUtf8 inner;
#endif
    };

}

#undef NS_YYCC_PATCH_EXPECTED
#undef NS_YYCC_STRING
