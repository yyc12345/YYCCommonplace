#include "stl.hpp"
#include <locale>

namespace yycc::encoding::stl {

#pragma region Generic Converter

    /*
     * YYC MARK:
     * According to the documentation introduced in CppReference.
     * The standard library is guaranteed to provide several specific specializations of \c std::codecvt.
     * The UTF8 char type in UTF8 related specializations of \c std::codecvt is different in different C++ standard.
     * But the oldest C++ version YYCC supported is C++ 23, char8_t is the only viable UTF8 char type for \c std::codecvt.
     * So we can simply and safely use it to correctly trigger specific specializations of \c std::codecv in there.
    */

    template<typename TChar>
        requires(std::is_same_v<TChar, char16_t> || std::is_same_v<TChar, char32_t>)
    using CodecvtFacet = std::codecvt<TChar, char8_t, std::mbstate_t>;

    template<typename TChar>
        requires(std::is_same_v<TChar, char16_t> || std::is_same_v<TChar, char32_t>)
    static ConvResult<std::basic_string<TChar>> generic_to_utf_other(const std::u8string_view& src) {
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
        const char8_t *intern_from = reinterpret_cast<const char8_t*>(src.data()),
                      *intern_from_end = reinterpret_cast<const char8_t*>(src.data() + src.size()), *intern_from_next = nullptr;
        TChar *extern_to = dst.data(), *extern_to_end = dst.data() + dst.size(), *extern_to_next = nullptr;
        // do convertion
        auto result = this_codecvt.in(mb, intern_from, intern_from_end, intern_from_next, extern_to, extern_to_end, extern_to_next);

        // check result
        if (result != CodecvtFacet<TChar>::ok) return std::unexpected(ConvError{});
        // resize result and return
        dst.resize(extern_to_next - dst.data());
        return dst;
    }

    template<typename TChar>
        requires(std::is_same_v<TChar, char16_t> || std::is_same_v<TChar, char32_t>)
    static ConvResult<std::u8string> generic_to_utf8(const std::basic_string_view<TChar>& src) {
        // Reference:
        // https://en.cppreference.com/w/cpp/locale/codecvt/out

        // prepare return value
        std::u8string dst;

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
        const TChar *intern_from = src.data(), *intern_from_end = src.data() + src.size(), *intern_from_next = nullptr;
        char8_t *extern_to = reinterpret_cast<char8_t*>(dst.data()), *extern_to_end = reinterpret_cast<char8_t*>(dst.data() + dst.size()),
                *extern_to_next = nullptr;
        // do convertion
        auto result = this_codecvt.out(mb, intern_from, intern_from_end, intern_from_next, extern_to, extern_to_end, extern_to_next);

        // check result
        if (result != CodecvtFacet<TChar>::ok) return std::unexpected(ConvError{});
        // resize result and retuen
        dst.resize(extern_to_next - reinterpret_cast<char8_t*>(dst.data()));
        return dst;
    }

#pragma endregion Converter

#pragma region

    ConvResult<std::u16string> to_utf16(const std::u8string_view& src) {
        // UTF8 -> UTF16
        return generic_to_utf_other<char16_t>(src);
    }

    ConvResult<std::u8string> to_utf8(const std::u16string_view& src) {
        // UTF16 -> UTF8
        return generic_to_utf8<char16_t>(src);
    }

    ConvResult<std::u32string> to_utf32(const std::u8string_view& src) {
        // UTF8 -> UTF32
        return generic_to_utf_other<char32_t>(src);
    }

    ConvResult<std::u8string> to_utf8(const std::u32string_view& src) {
        // UTF32 -> UTF8
        return generic_to_utf8<char32_t>(src);
    }

#pragma endregion

} // namespace yycc::encoding::stlcvt
