#pragma once
#include "../macro/os_detector.hpp"
#include "../macro/stl_detector.hpp"
#include "../macro/class_copy_move.hpp"
#include <string>
#include <string_view>
#include <expected>

// Choose the backend of PyCodec module
#if defined(YYCC_FEAT_ICONV)
// We try Iconv first in any cases.
#include "../encoding/iconv.hpp"
#define YYCC_PYCODEC_ICONV_BACKEND
#define YYCC_PYCODEC_BACKEND_NS ::yycc::encoding::iconv
#elif defined(YYCC_OS_WINDOWS) && defined(YYCC_STL_MSSTL)
// If we can not use Iconv, we try to fallback to Windows implementation.
#include "../encoding/windows.hpp"
#define YYCC_PYCODEC_WIN32_BACKEND
#define YYCC_PYCODEC_BACKEND_NS ::yycc::encoding::windows
#else
// No viable implementation.
#error "Can not find viable encoding convertion solution in current environment for PyCodec module."
#endif

namespace yycc::carton::pycodec {

    /// @brief The universal name of encoding.
    using EncodingName = std::u8string_view;

    /// @brief The possible error occurs in this module.
    class ConvError {
    public:
        using Error = YYCC_PYCODEC_BACKEND_NS::ConvError;
        ConvError(const Error& err);
        YYCC_DEFAULT_COPY_MOVE(ConvError)

    private:
        Error inner;
    };

    /// @brief The result type of this module.
    template<typename T>
    using ConvResult = std::expected<T, ConvError>;

    /**
     * @brief Check whether given name is a valid encoding name in PyCodec.
     * @param[in] name The name to be checked.
     * @return True if it is valid, otherwise false.
     */
    bool is_valid_encoding_name(const EncodingName& name);

    /// @brief Char -> UTF8
    class CharToUtf8 {
    public:
        CharToUtf8(const EncodingName& name);
        ~CharToUtf8();
        YYCC_DELETE_COPY(CharToUtf8)
        YYCC_DEFAULT_MOVE(CharToUtf8)

    public:
        ConvResult<std::u8string> to_utf8(const std::string_view& src);

    private:
#if defined(YYCC_PYCODEC_WIN32_BACKEND)
        YYCC_PYCODEC_BACKEND_NS::CodePage code_page;
#else
        YYCC_PYCODEC_BACKEND_NS::CharToUtf8 inner;
#endif
    };

    /// @brief UTF8 -> Char
    class Utf8ToChar {
    public:
        Utf8ToChar(const EncodingName& name);
        ~Utf8ToChar();
        YYCC_DELETE_COPY(Utf8ToChar)
        YYCC_DEFAULT_MOVE(Utf8ToChar)

    public:
        ConvResult<std::string> to_char(const std::u8string_view& src);

    private:
#if defined(YYCC_PYCODEC_WIN32_BACKEND)
        YYCC_PYCODEC_BACKEND_NS::CodePage code_page;
#else
        YYCC_PYCODEC_BACKEND_NS::Utf8ToChar inner;
#endif
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
#if defined(YYCC_PYCODEC_ICONV_BACKEND)
        YYCC_PYCODEC_BACKEND_NS::WcharToUtf8 inner;
#endif
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
#if defined(YYCC_PYCODEC_ICONV_BACKEND)
        YYCC_PYCODEC_BACKEND_NS::Utf8ToWchar inner;
#endif
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
#if defined(YYCC_PYCODEC_ICONV_BACKEND)
        YYCC_PYCODEC_BACKEND_NS::Utf8ToUtf16 inner;
#endif
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
#if defined(YYCC_PYCODEC_ICONV_BACKEND)
        YYCC_PYCODEC_BACKEND_NS::Utf16ToUtf8 inner;
#endif
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
#if defined(YYCC_PYCODEC_ICONV_BACKEND)
        YYCC_PYCODEC_BACKEND_NS::Utf8ToUtf32 inner;
#endif
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
#if defined(YYCC_PYCODEC_ICONV_BACKEND)
        YYCC_PYCODEC_BACKEND_NS::Utf32ToUtf8 inner;
#endif
    };

} // namespace yycc::encoding::pycodec
