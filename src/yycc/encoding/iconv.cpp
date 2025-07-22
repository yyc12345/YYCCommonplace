#include "iconv.hpp"

#if YYCC_FEAT_ICONV || (YYCC_OS != YYCC_OS_WINDOWS)

#include "../string/reinterpret.hpp"
#include <cerrno>
#include <stdexcept>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <iconv.h>

#define NS_YYCC_STRING ::yycc::string
#define NS_YYCC_STRING_REINTERPRET ::yycc::string::reinterpret
#define NS_YYCC_PATCH_EXPECTED ::yycc::patch::expected

#pragma region Iconv Shit Fix

// YYC MARK:
// I don't know what Iconv is for, Iconv put an huge pieces of shit into its header file "iconv.h" (at least for me).
// Especially a macro called iconv, which pollutes my namespace name while also can not be disabled because I need to rely on it to access essential functions.
// I can't simply redefine it, because I can't make sure that this "iconv" is defined in that way on all platforms.
// So I can only write some definitions of functions and types here, and extract the functions and types I need before I declare the namespace.
// And at the same time remove those annoying macro definitions. Hopefully, the compiler will optimize these wrapper functions.

typedef iconv_t that_iconv_t;
static iconv_t that_iconv_open(const char* tocode, const char* fromcode) {
    return iconv_open(tocode, fromcode);
}
static int that_iconv_close(iconv_t cd) {
    return iconv_close(cd);
}
static size_t that_iconv(iconv_t cd, const char** inbuf, size_t* inbytesleft, char** outbuf, size_t* outbytesleft) {
    // YYC MARK:
    // This is also bullshit. I don't know why the real signature of this function differ with its document written by GNU.
    // I have to make a "const" cast in there.
    return iconv(cd, const_cast<char**>(inbuf), inbytesleft, outbuf, outbytesleft);
}

#undef iconv_t
#undef iconv_open
#undef iconv_close
#undef iconv

#pragma endregion

namespace yycc::encoding::iconv {

    static const that_iconv_t INVALID_ICONV_TOKEN = reinterpret_cast<that_iconv_t>(-1);

#pragma region PrivToken

    class PrivToken {
    public:
        PrivToken(const CodeName& from_code, const CodeName& to_code) : inner(INVALID_ICONV_TOKEN) {
            // We must cast them into string container, not string view,
            // because they may not have NULL terminator.
            std::string iconv_from_code = NS_YYCC_STRING_REINTERPRET::as_ordinary(from_code),
                        iconv_to_code = NS_YYCC_STRING_REINTERPRET::as_ordinary(to_code);
            // Call iconv_t creator
            that_iconv_t descriptor = that_iconv_open(iconv_to_code.c_str(), iconv_from_code.c_str());
            if (descriptor == INVALID_ICONV_TOKEN) {
                if (errno == EINVAL) {
                    return;
                } else {
                    throw std::runtime_error("impossible errno when calling iconv_open()");
                }
            }
            // Setup value
            this->inner = descriptor;
        }
        ~PrivToken() {
            if (this->inner != INVALID_ICONV_TOKEN) {
                that_iconv_close(this->inner);
            }
        }
        PrivToken(PrivToken&& rhs) : inner(rhs.inner) {
            // Reset rhs inner
            rhs.inner = INVALID_ICONV_TOKEN;
        }
        PrivToken& operator=(PrivToken&& rhs) {
            // Free self first
            if (this->inner != INVALID_ICONV_TOKEN) {
                that_iconv_close(this->inner);
            }
            // Copy rhs inner and reset it.
            this->inner = rhs.inner;
            rhs.inner = INVALID_ICONV_TOKEN;
            // Return self
            return *this;
        }
        YYCC_DELETE_COPY(PrivToken)

        bool is_valid() const { return this->inner != INVALID_ICONV_TOKEN; }
        that_iconv_t get_inner() const { return this->inner; }

    private:
        that_iconv_t inner;
    };

#pragma endregion

#pragma region Token

    Token::Token(const CodeName& from_code, const CodeName& to_code) : inner(std::make_unique<PrivToken>(from_code, to_code)) {}

    Token::~Token() {}

    bool Token::is_valid() const {
        return this->inner->is_valid();
    }

    PrivToken* Token::get_inner() const {
        return this->inner.get();
    }

#pragma endregion

#pragma region Kernel

    constexpr const size_t ICONV_INC_LEN = 16u;
    constexpr size_t ICONV_ERR_RV = static_cast<size_t>(-1);

    // Reference: https://stackoverflow.com/questions/13297458/simple-utf8-utf16-string-conversion-with-iconv

    static ConvResult<std::vector<uint8_t>> iconv_kernel(const Token& token, const uint8_t* str_from_buf, size_t str_from_len) {
        // ===== Check Requirements =====
        // Prepare return value
        std::vector<uint8_t> str_to;

        // Unwrap and check iconv_t
        that_iconv_t cd = token.get_inner()->get_inner();
        if (cd == INVALID_ICONV_TOKEN) return ConvError::InvalidCd;

        // Check empty input
        if (str_from_len == 0u) return str_to;
        // Check nullptr input variables
        if (str_from_buf == nullptr) return ConvError::NullPointer;

        // ===== Do Iconv =====
        // setup input variables
        size_t inbytesleft = str_from_len;
        const char* inbuf = reinterpret_cast<const char*>(str_from_buf);
        // pre-allocation output variables
        str_to.resize(str_from_len + ICONV_INC_LEN);
        size_t outbytesleft = str_to.size();
        char* outbuf = reinterpret_cast<char*>(str_to.data());

        // conv core
        size_t nchars = that_iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
        while (nchars == ICONV_ERR_RV && errno == E2BIG) {
            // record the length has been converted
            size_t len = outbuf - reinterpret_cast<char*>(str_to.data());

            // resize for container and its variables
            str_to.resize(str_to.size() + ICONV_INC_LEN);
            outbytesleft = str_to.size();

            // assign new outbuf from failed position
            outbuf = reinterpret_cast<char*>(str_to.data()) + len;
            nchars = that_iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
        }

        // restore descriptor initial state
        that_iconv(cd, nullptr, nullptr, nullptr, nullptr);

        // check error
        if (nchars == ICONV_ERR_RV) {
            if (errno == EILSEQ) {
                return ConvError::InvalidMbSeq;
            } else if (errno == EINVAL) {
                return ConvError::IncompleteMbSeq;
            } else {
                throw std::runtime_error("impossible errno when calling iconv_open()");
            }
        } else {
            // success
            // compute result data
            str_to.resize(str_to.size() - outbytesleft);
            return str_to;
        }
    }

#pragma endregion

#pragma region Convertion Class Helper

    // YYC MARK:
    // If we use UTF16 or UTF32 code name directly, it will produce a BOM at data head.
    // That's not what we expected.
    // So we need manually check runtime endian and explicitly specify endian in code name.

    // TODO: fix this encoding endian issue.

    static const NS_YYCC_STRING::u8char* UTF8_CODENAME_LITERAL = YYCC_U8("UTF-8");
    static const NS_YYCC_STRING::u8char* WCHAR_CODENAME_LITERAL = YYCC_U8("WCHAR_T");
    static const NS_YYCC_STRING::u8char* fetch_utf16_codename() {
        return YYCC_U8("UTF16");
    }
    static const NS_YYCC_STRING::u8char* UTF16_CODENAME_LITERAL = fetch_utf16_codename();
    static const NS_YYCC_STRING::u8char* fetch_utf32_codename() {
        return YYCC_U8("UTF32");
    }
    static const NS_YYCC_STRING::u8char* UTF32_CODENAME_LITERAL = fetch_utf32_codename();

    // TODO: There is a memory copy in this function. Consider removing it in future.
#define CONVFN_TYPE0(src_char_type, dst_char_type) \
    namespace expected = NS_YYCC_PATCH_EXPECTED; \
    auto rv = iconv_kernel(this->token, reinterpret_cast<const uint8_t*>(src.data()), src.size()); \
    if (expected::is_value(rv)) { \
        const auto& dst = expected::get_value(rv); \
        if constexpr (sizeof(dst_char_type) > 1u) { \
            if (dst.size() % sizeof(dst_char_type) != 0u) return ConvError::BadRv; \
        } \
        return std::basic_string<dst_char_type>(reinterpret_cast<const dst_char_type*>(dst.data()), dst.size() / sizeof(dst_char_type)); \
    } else { \
        return expected::get_error(rv); \
    }

#define CONVFN_TYPE1(fct_name, src_char_type, dst_char_type) \
    namespace expected = NS_YYCC_PATCH_EXPECTED; \
    auto rv = this->priv_##fct_name(src); \
    if (expected::is_value(rv)) { \
        dst = std::move(expected::get_value(rv)); \
        return true; \
    } else { \
        return false; \
    }

#define CONVFN_TYPE2(fct_name, src_char_type, dst_char_type) \
    std::basic_string<dst_char_type> rv; \
    if (this->fct_name(src, rv)) return rv; \
    else throw std::runtime_error("fail to convert string in Win32 function");

#pragma endregion

#pragma region Char -> UTF8

    CharToUtf8::CharToUtf8(const CodeName& code_name) : token(code_name, UTF8_CODENAME_LITERAL) {}

    CharToUtf8::~CharToUtf8() {}

    ConvResult<NS_YYCC_STRING::u8string> CharToUtf8::priv_to_utf8(const std::string_view& src) {
        CONVFN_TYPE0(char, NS_YYCC_STRING::u8char);
    }

    bool CharToUtf8::to_utf8(const std::string_view& src, NS_YYCC_STRING::u8string& dst) {
        CONVFN_TYPE1(to_utf8, char, NS_YYCC_STRING::u8char);
    }

    NS_YYCC_STRING::u8string CharToUtf8::to_utf8(const std::string_view& src) {
        CONVFN_TYPE2(to_utf8, char, NS_YYCC_STRING::u8char);
    }

#pragma endregion

#pragma region UTF8 -> Char

    Utf8ToChar::Utf8ToChar(const CodeName& code_name) : token(UTF8_CODENAME_LITERAL, code_name) {}

    Utf8ToChar::~Utf8ToChar() {}

    ConvResult<std::string> Utf8ToChar::priv_to_char(const NS_YYCC_STRING::u8string_view& src) {
        CONVFN_TYPE0(NS_YYCC_STRING::u8char, char);
    }

    bool Utf8ToChar::to_char(const NS_YYCC_STRING::u8string_view& src, std::string& dst) {
        CONVFN_TYPE1(to_char, NS_YYCC_STRING::u8char, char);
    }

    std::string Utf8ToChar::to_char(const NS_YYCC_STRING::u8string_view& src) {
        CONVFN_TYPE2(to_char, NS_YYCC_STRING::u8char, char);
    }

#pragma endregion

#pragma region WChar -> Char

    WcharToUtf8::WcharToUtf8() : token(WCHAR_CODENAME_LITERAL, UTF8_CODENAME_LITERAL) {}

    WcharToUtf8::~WcharToUtf8() {}

    ConvResult<NS_YYCC_STRING::u8string> WcharToUtf8::priv_to_utf8(const std::wstring_view& src) {
        CONVFN_TYPE0(wchar_t, NS_YYCC_STRING::u8char);
    }

    bool WcharToUtf8::to_utf8(const std::wstring_view& src, NS_YYCC_STRING::u8string& dst) {
        CONVFN_TYPE1(to_utf8, wchar_t, NS_YYCC_STRING::u8char);
    }

    NS_YYCC_STRING::u8string WcharToUtf8::to_utf8(const std::wstring_view& src) {
        CONVFN_TYPE2(to_utf8, wchar_t, NS_YYCC_STRING::u8char);
    }

#pragma endregion

#pragma region Char -> WChar

    Utf8ToWchar::Utf8ToWchar() : token(UTF8_CODENAME_LITERAL, WCHAR_CODENAME_LITERAL) {}

    Utf8ToWchar::~Utf8ToWchar() {}

    ConvResult<std::wstring> Utf8ToWchar::priv_to_wchar(const NS_YYCC_STRING::u8string_view& src) {
        CONVFN_TYPE0(NS_YYCC_STRING::u8char, wchar_t);
    }

    bool Utf8ToWchar::to_wchar(const NS_YYCC_STRING::u8string_view& src, std::wstring& dst) {
        CONVFN_TYPE1(to_wchar, NS_YYCC_STRING::u8char, wchar_t);
    }

    std::wstring Utf8ToWchar::to_wchar(const NS_YYCC_STRING::u8string_view& src) {
        CONVFN_TYPE2(to_wchar, NS_YYCC_STRING::u8char, wchar_t);
    }

#pragma endregion

#pragma region UTF8 -> UTF16

    Utf8ToUtf16::Utf8ToUtf16() : token(UTF8_CODENAME_LITERAL, UTF16_CODENAME_LITERAL) {}

    Utf8ToUtf16::~Utf8ToUtf16() {}

    ConvResult<std::u16string> Utf8ToUtf16::priv_to_utf16(const NS_YYCC_STRING::u8string_view& src) {
        CONVFN_TYPE0(NS_YYCC_STRING::u8char, char16_t);
    }

    bool Utf8ToUtf16::to_utf16(const NS_YYCC_STRING::u8string_view& src, std::u16string& dst) {
        CONVFN_TYPE1(to_utf16, NS_YYCC_STRING::u8char, char16_t);
    }

    std::u16string Utf8ToUtf16::to_utf16(const NS_YYCC_STRING::u8string_view& src) {
        CONVFN_TYPE2(to_utf16, NS_YYCC_STRING::u8char, char16_t);
    }

#pragma endregion

#pragma region UTF16 -> UTF8

    Utf16ToUtf8::Utf16ToUtf8() : token(UTF16_CODENAME_LITERAL, UTF8_CODENAME_LITERAL) {}

    Utf16ToUtf8::~Utf16ToUtf8() {}

    ConvResult<NS_YYCC_STRING::u8string> Utf16ToUtf8::priv_to_utf8(const std::u16string_view& src) {
        CONVFN_TYPE0(char16_t, NS_YYCC_STRING::u8char);
    }

    bool Utf16ToUtf8::to_utf8(const std::u16string_view& src, NS_YYCC_STRING::u8string& dst) {
        CONVFN_TYPE1(to_utf8, char16_t, NS_YYCC_STRING::u8char);
    }

    NS_YYCC_STRING::u8string Utf16ToUtf8::to_utf8(const std::u16string_view& src) {
        CONVFN_TYPE2(to_utf8, char16_t, NS_YYCC_STRING::u8char);
    }

#pragma endregion

#pragma region UTF8 -> UTF32

    Utf8ToUtf32::Utf8ToUtf32() : token(UTF8_CODENAME_LITERAL, UTF32_CODENAME_LITERAL) {}

    Utf8ToUtf32::~Utf8ToUtf32() {}

    ConvResult<std::u32string> Utf8ToUtf32::priv_to_utf32(const NS_YYCC_STRING::u8string_view& src) {
        CONVFN_TYPE0(NS_YYCC_STRING::u8char, char32_t);
    }

    bool Utf8ToUtf32::to_utf32(const NS_YYCC_STRING::u8string_view& src, std::u32string& dst) {
        CONVFN_TYPE1(to_utf32, NS_YYCC_STRING::u8char, char32_t);
    }

    std::u32string Utf8ToUtf32::to_utf32(const NS_YYCC_STRING::u8string_view& src) {
        CONVFN_TYPE2(to_utf32, NS_YYCC_STRING::u8char, char32_t);
    }

#pragma endregion

#pragma region UTF32 -> UTF8

    Utf32ToUtf8::Utf32ToUtf8() : token(UTF32_CODENAME_LITERAL, UTF8_CODENAME_LITERAL) {}

    Utf32ToUtf8::~Utf32ToUtf8() {}

    ConvResult<NS_YYCC_STRING::u8string> Utf32ToUtf8::priv_to_utf8(const std::u32string_view& src) {
        CONVFN_TYPE0(char32_t, NS_YYCC_STRING::u8char);
    }

    bool Utf32ToUtf8::to_utf8(const std::u32string_view& src, NS_YYCC_STRING::u8string& dst) {
        CONVFN_TYPE1(to_utf8, char32_t, NS_YYCC_STRING::u8char);
    }

    NS_YYCC_STRING::u8string Utf32ToUtf8::to_utf8(const std::u32string_view& src) {
        CONVFN_TYPE2(to_utf8, char32_t, NS_YYCC_STRING::u8char);
    }

#pragma endregion

} // namespace yycc::encoding::iconv

#endif
