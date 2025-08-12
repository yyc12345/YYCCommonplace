#include "literals.hpp"
#include <stdexcept>

namespace yyccshared::literals {

#define CONCAT(prefix, strl) prefix##strl
#define U8_LITERAL(strl) CONCAT(u8, strl)
#define U16_LITERAL(strl) CONCAT(u, strl)
#define U32_LITERAL(strl) CONCAT(U, strl)
#define WSTR_LITERAL(strl) CONCAT(L, strl)

#pragma region UtfLiterals Data

    // UNICODE Test Strings
    // Ref: https://stackoverflow.com/questions/478201/how-to-test-an-application-for-correct-encoding-e-g-utf-8

#define UNICODE_STR_JAPAN "\u30E6\u30FC\u30B6\u30FC\u5225\u30B5\u30A4\u30C8"
#define UNICODE_STR_CHINA "\u7B80\u4F53\u4E2D\u6587"
#define UNICODE_STR_KOREA "\uD06C\uB85C\uC2A4 \uD50C\uB7AB\uD3FC\uC73C\uB85C"
#define UNICODE_STR_ISRAEL "\u05DE\u05D3\u05D5\u05E8\u05D9\u05DD \u05DE\u05D1\u05D5\u05E7\u05E9\u05D9\u05DD"
#define UNICODE_STR_EGYPT "\u0623\u0641\u0636\u0644 \u0627\u0644\u0628\u062D\u0648\u062B"
#define UNICODE_STR_GREECE "\u03A3\u1F72 \u03B3\u03BD\u03C9\u03C1\u03AF\u03B6\u03C9 \u1F00\u03C0\u1F78"
#define UNICODE_STR_RUSSIA \
    "\u0414\u0435\u0441\u044F\u0442\u0443\u044E \u041C\u0435\u0436\u0434\u0443\u043D\u0430\u0440\u043E\u0434\u043D\u0443\u044E"
#define UNICODE_STR_THAILAND \
    "\u0E41\u0E1C\u0E48\u0E19\u0E14\u0E34\u0E19\u0E2E\u0E31\u0E48\u0E19\u0E40\u0E2A\u0E37\u0E48\u0E2D\u0E21\u0E42\u0E17\u0E23\u0E21\u0E41\u0E2A\u0E19\u0E2A\u0E31\u0E07\u0E40\u0E27\u0E0A"
#define UNICODE_STR_FRANCE "fran\u00E7ais langue \u00E9trang\u00E8re"
#define UNICODE_STR_SPAIN "ma\u00F1ana ol\u00E9"
#define UNICODE_STR_MATHMATICS "\u222E E\u22C5da = Q,  n \u2192 \u221E, \u2211 f(i) = \u220F g(i)"
#define UNICODE_STR_EMOJI "\U0001F363 \u2716 \U0001F37A" // sushi x beer mug

    static std::vector<std::u8string> UTFLIT_U8STR_VEC{
        U8_LITERAL(UNICODE_STR_JAPAN),
        U8_LITERAL(UNICODE_STR_CHINA),
        U8_LITERAL(UNICODE_STR_KOREA),
        U8_LITERAL(UNICODE_STR_ISRAEL),
        U8_LITERAL(UNICODE_STR_EGYPT),
        U8_LITERAL(UNICODE_STR_GREECE),
        U8_LITERAL(UNICODE_STR_RUSSIA),
        U8_LITERAL(UNICODE_STR_THAILAND),
        U8_LITERAL(UNICODE_STR_FRANCE),
        U8_LITERAL(UNICODE_STR_SPAIN),
        U8_LITERAL(UNICODE_STR_MATHMATICS),
        U8_LITERAL(UNICODE_STR_EMOJI),
    };
    static std::vector<std::wstring> UTFLIT_WSTR_VEC{
        WSTR_LITERAL(UNICODE_STR_JAPAN),
        WSTR_LITERAL(UNICODE_STR_CHINA),
        WSTR_LITERAL(UNICODE_STR_KOREA),
        WSTR_LITERAL(UNICODE_STR_ISRAEL),
        WSTR_LITERAL(UNICODE_STR_EGYPT),
        WSTR_LITERAL(UNICODE_STR_GREECE),
        WSTR_LITERAL(UNICODE_STR_RUSSIA),
        WSTR_LITERAL(UNICODE_STR_THAILAND),
        WSTR_LITERAL(UNICODE_STR_FRANCE),
        WSTR_LITERAL(UNICODE_STR_SPAIN),
        WSTR_LITERAL(UNICODE_STR_MATHMATICS),
        WSTR_LITERAL(UNICODE_STR_EMOJI),
    };
    static std::vector<std::u16string> UTFLIT_U16STR_VEC{
        U16_LITERAL(UNICODE_STR_JAPAN),
        U16_LITERAL(UNICODE_STR_CHINA),
        U16_LITERAL(UNICODE_STR_KOREA),
        U16_LITERAL(UNICODE_STR_ISRAEL),
        U16_LITERAL(UNICODE_STR_EGYPT),
        U16_LITERAL(UNICODE_STR_GREECE),
        U16_LITERAL(UNICODE_STR_RUSSIA),
        U16_LITERAL(UNICODE_STR_THAILAND),
        U16_LITERAL(UNICODE_STR_FRANCE),
        U16_LITERAL(UNICODE_STR_SPAIN),
        U16_LITERAL(UNICODE_STR_MATHMATICS),
        U16_LITERAL(UNICODE_STR_EMOJI),
    };
    static std::vector<std::u32string> UTFLIT_U32STR_VEC{
        U32_LITERAL(UNICODE_STR_JAPAN),
        U32_LITERAL(UNICODE_STR_CHINA),
        U32_LITERAL(UNICODE_STR_KOREA),
        U32_LITERAL(UNICODE_STR_ISRAEL),
        U32_LITERAL(UNICODE_STR_EGYPT),
        U32_LITERAL(UNICODE_STR_GREECE),
        U32_LITERAL(UNICODE_STR_RUSSIA),
        U32_LITERAL(UNICODE_STR_THAILAND),
        U32_LITERAL(UNICODE_STR_FRANCE),
        U32_LITERAL(UNICODE_STR_SPAIN),
        U32_LITERAL(UNICODE_STR_MATHMATICS),
        U32_LITERAL(UNICODE_STR_EMOJI),
    };

#pragma endregion

#pragma region UtfLiterals

    UtfLiterals::UtfLiterals() :
        u8str_vec(UTFLIT_U8STR_VEC), u16str_vec(UTFLIT_U16STR_VEC), u32str_vec(UTFLIT_U32STR_VEC), wstr_vec(UTFLIT_WSTR_VEC) {
        // Check whether each vector has same size.
        bool okey = true;
        size_t exp_size = this->u8str_vec.size();

        if (this->u16str_vec.size() != exp_size) okey = false;
        if (this->u32str_vec.size() != exp_size) okey = false;
        if (this->wstr_vec.size() != exp_size) okey = false;

        if (!okey) throw std::logic_error("utf literal vector have different size");
    }

    UtfLiterals::~UtfLiterals() {}

    size_t UtfLiterals::get_size() const {
        // We have checked the size of each vector in ctor.
        // So we simply return the length of one of them.
        return this->u8str_vec.size();
    }

    const std::vector<std::u8string> &UtfLiterals::get_u8str_vec() const {
        return this->u8str_vec;
    }

    const std::vector<std::u16string> &UtfLiterals::get_u16str_vec() const {
        return this->u16str_vec;
    }

    const std::vector<std::u32string> &UtfLiterals::get_u32str_vec() const {
        return this->u32str_vec;
    }

    const std::vector<std::wstring> &UtfLiterals::get_wstr_vec() const {
        return this->wstr_vec;
    }

#pragma endregion

#pragma region OtherLiteral

    OtherLiteral::OtherLiteral(std::string &&other_str, uint32_t windows_ident, std::string &&iconv_ident, std::u8string &&pycodec_ident) :
        other_str(std::move(other_str)), windows_ident(windows_ident), iconv_ident(std::move(iconv_ident)),
        pycodec_ident(std::move(pycodec_ident)) {}

    OtherLiteral::~OtherLiteral() {}

    const std::string &OtherLiteral::get_other_str() const {
        return this->other_str;
    }

    uint32_t OtherLiteral::get_windows_ident() const {
        return this->windows_ident;
    }

    const std::string &OtherLiteral::get_iconv_ident() const {
        return this->iconv_ident;
    }

    const std::u8string &OtherLiteral::get_pycodec_ident() const {
        return this->pycodec_ident;
    }

#pragma endregion

#pragma region OtherLiterals Data

    std::vector<OtherLiteral> OTHERLIT_OTHERSTR_VEC{{"\xC4\xE3\xBA\xC3\xD6\xD0\xB9\xFA", UINT32_C(936), "GBK", u8"gbk"}};

#define OTHER_STR_GBK "\u4f60\u597d\u4e2d\u56fd"

    static std::vector<std::u8string> OTHERLIT_U8STR_VEC{U8_LITERAL(OTHER_STR_GBK)};
    static std::vector<std::wstring> OTHERLIT_WSTR_VEC{WSTR_LITERAL(OTHER_STR_GBK)};

#pragma endregion

#pragma region OtherLiterals

    OtherLiterals::OtherLiterals() : other_str_vec(OTHERLIT_OTHERSTR_VEC), u8str_vec(OTHERLIT_U8STR_VEC), wstr_vec(OTHERLIT_WSTR_VEC) {
        // Check whether each vector has same size.
        bool okey = true;
        size_t exp_size = this->other_str_vec.size();

        if (this->u8str_vec.size() != exp_size) okey = false;
        if (this->wstr_vec.size() != exp_size) okey = false;

        if (!okey) throw std::logic_error("utf literal vector have different size");
    }

    OtherLiterals::~OtherLiterals() {}

    size_t OtherLiterals::get_size() const {
        // We have checked the size, return size directly.
        return this->other_str_vec.size();
    }

    const std::vector<OtherLiteral> &OtherLiterals::get_other_str_vec() const {
        return this->other_str_vec;
    }

    const std::vector<std::u8string> &OtherLiterals::get_u8str_vec() const {
        return this->u8str_vec;
    }

    const std::vector<std::wstring> &OtherLiterals::get_wstr_vec() const {
        return this->wstr_vec;
    }

#pragma endregion

} // namespace yyccshared::literals
