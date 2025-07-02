#include "utf.hpp"
#include "../macro/feature_probe.hpp"
#include <locale>

#define NS_YYCC_STRING ::yycc::string

namespace yycc::encoding::utf {

#pragma region Generic Converter

    /*
     * NOTE:
     * According to the documentation introduced in CppReference.
     * The standard library is guaranteed to provide several specific specializations of \c std::codecvt.
     * The UTF8 char type in UTF8 related specializations of \c std::codecvt is different.
     * It is also independend from we defined char type.
     * So it is essential define a type which can correctly trigger specific specializations of \c std::codecv in there.
    */

#if defined(YYCC_CPPFEAT_UTF8)
    using CodecvtUtf8Char = char8_t;
#else
    using CodecvtUtf8Char = char;
#endif

    template<typename TChar,
             std::enable_if_t<std::is_same_v<TChar, char16_t> || std::is_same_v<TChar, char32_t>, int>
             = 0>
    using CodecvtFacet = std::codecvt<TChar, CodecvtUtf8Char, std::mbstate_t>;

    template<typename TChar,
             std::enable_if_t<std::is_same_v<TChar, char16_t> || std::is_same_v<TChar, char32_t>, int>
             = 0>
    static ConvResult<std::basic_string<TChar>> generic_to_utf_other(
        const NS_YYCC_STRING::u8string_view& src) {
        // Reference:
        // https://en.cppreference.com/w/cpp/locale/codecvt/in

        // prepare return value
        std::basic_string<TChar> dst;

        // if src is empty, return directly
        if (src.empty()) {
            return dst;
        }

        // init locale and get codecvt facet
        // same reason in UTFOtherToUTF8 to keeping reference to locale
        const auto& this_locale = std::locale::classic();
        const auto& this_codecvt = std::use_facet<CodecvtFacet<TChar>>(this_locale);

        // convertion preparation
        std::mbstate_t mb{};
        dst.resize(src.size());
        const CodecvtUtf8Char *intern_from = reinterpret_cast<const CodecvtUtf8Char*>(src.data()),
                              *intern_from_end = reinterpret_cast<const CodecvtUtf8Char*>(
                                  src.data() + src.size()),
                              *intern_from_next = nullptr;
        TChar *extern_to = dst.data(), *extern_to_end = dst.data() + dst.size(),
              *extern_to_next = nullptr;
        // do convertion
        auto result = this_codecvt.in(mb,
                                      intern_from,
                                      intern_from_end,
                                      intern_from_next,
                                      extern_to,
                                      extern_to_end,
                                      extern_to_next);

        // check result
        if (result != CodecvtFacet<TChar>::ok) return ConvError();
        // resize result and return
        dst.resize(extern_to_next - dst.data());
        return dst;
    }

    template<typename TChar,
             std::enable_if_t<std::is_same_v<TChar, char16_t> || std::is_same_v<TChar, char32_t>, int>
             = 0>
    static ConvResult<NS_YYCC_STRING::u8string> generic_to_utf8(
        const std::basic_string_view<TChar>& src) {
        // Reference:
        // https://en.cppreference.com/w/cpp/locale/codecvt/out

        // prepare return value
        NS_YYCC_STRING::u8string dst;

        // if src is empty, return directly
        if (src.empty()) {
            return dst;
        }

        // init locale and get codecvt facet
        // the reference to locale must be preserved until convertion done.
        // because the life time of codecvt facet is equal to the reference to locale.
        const auto& this_locale = std::locale::classic();
        const auto& this_codecvt = std::use_facet<CodecvtFacet<TChar>>(this_locale);

        // do convertion preparation
        std::mbstate_t mb{};
        dst.resize(src.size() * this_codecvt.max_length());
        const TChar *intern_from = src.data(), *intern_from_end = src.data() + src.size(),
                    *intern_from_next = nullptr;
        CodecvtUtf8Char *extern_to = reinterpret_cast<CodecvtUtf8Char*>(dst.data()),
                        *extern_to_end = reinterpret_cast<CodecvtUtf8Char*>(dst.data() + dst.size()),
                        *extern_to_next = nullptr;
        // do convertion
        auto result = this_codecvt.out(mb,
                                       intern_from,
                                       intern_from_end,
                                       intern_from_next,
                                       extern_to,
                                       extern_to_end,
                                       extern_to_next);

        // check result
        if (result != CodecvtFacet<TChar>::ok) return ConvError();
        // resize result and retuen
        dst.resize(extern_to_next - reinterpret_cast<CodecvtUtf8Char*>(dst.data()));
        return dst;
    }

#pragma endregion

#pragma region Help Macros

#define CONVFN_TYPE1(fct_name, src_char_type, dst_char_type) \
    auto rv = priv_##fct_name(src); \
    if (const auto* ptr = std::get_if<std::basic_string<dst_char_type>>(&rv)) { \
        dst = std::move(*ptr); \
        return true; \
    } else if (const auto* ptr = std::get_if<ConvError>(&rv)) { \
        return false; \
    } else { \
        throw std::runtime_error("unreachable code"); \
    }

#define CONVFN_TYPE2(fct_name, src_char_type, dst_char_type) \
    std::basic_string<dst_char_type> rv; \
    if (fct_name(src, rv)) return rv; \
    else throw std::runtime_error("fail to convert utf string");

#pragma endregion

#pragma region UTF8 -> UTF16

    ConvResult<std::u16string> priv_to_utf16(const NS_YYCC_STRING::u8string_view& src) {
        return generic_to_utf_other<char16_t>(src);
    }
    bool to_utf16(const NS_YYCC_STRING::u8string_view& src, std::u16string& dst) {
        CONVFN_TYPE1(to_utf16, NS_YYCC_STRING::u8char, char16_t);
    }
    std::u16string to_utf16(const NS_YYCC_STRING::u8string_view& src) {
        CONVFN_TYPE2(to_utf16, NS_YYCC_STRING::u8char, char16_t);
    }

#pragma endregion

#pragma region UTF16 -> UTF8

    ConvResult<NS_YYCC_STRING::u8string> priv_to_utf8(const std::u16string_view& src) {
        return generic_to_utf8<char16_t>(src);
    }
    bool to_utf8(const std::u16string_view& src, NS_YYCC_STRING::u8string& dst) {
        CONVFN_TYPE1(to_utf8, char16_t, NS_YYCC_STRING::u8char);
    }
    NS_YYCC_STRING::u8string to_utf8(const std::u16string_view& src) {
        CONVFN_TYPE2(to_utf8, char16_t, NS_YYCC_STRING::u8char);
    }

#pragma endregion

#pragma region UTF8 -> UTF32

    ConvResult<std::u32string> priv_to_utf32(const NS_YYCC_STRING::u8string_view& src) {
        return generic_to_utf_other<char32_t>(src);
    }
    bool to_utf32(const NS_YYCC_STRING::u8string_view& src, std::u32string& dst) {
        CONVFN_TYPE1(to_utf32, NS_YYCC_STRING::u8char, char32_t);
    }
    std::u32string to_utf32(const NS_YYCC_STRING::u8string_view& src) {
        CONVFN_TYPE2(to_utf32, NS_YYCC_STRING::u8char, char32_t);
    }

#pragma endregion

#pragma region UTF32 -> UTF8

    ConvResult<NS_YYCC_STRING::u8string> priv_to_utf8(const std::u32string_view& src) {
        return generic_to_utf8<char32_t>(src);
    }
    bool to_utf8(const std::u32string_view& src, NS_YYCC_STRING::u8string& dst) {
        CONVFN_TYPE1(to_utf8, char32_t, NS_YYCC_STRING::u8char);
    }
    NS_YYCC_STRING::u8string to_utf8(const std::u32string_view& src) {
        CONVFN_TYPE2(to_utf8, char32_t, NS_YYCC_STRING::u8char);
    }

#pragma endregion

} // namespace yycc::encoding::utf
