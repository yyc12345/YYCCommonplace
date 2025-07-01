#pragma once
#include "../macro/feature_probe.hpp"
#include <string>
#include <string_view>

namespace yycc::patch::starts_ends_with {

    // Reference:
    // https://en.cppreference.com/w/cpp/string/basic_string_view/starts_with
    // https://en.cppreference.com/w/cpp/string/basic_string_view/ends_with
    // https://en.cppreference.com/w/cpp/string/basic_string/starts_with
    // https://en.cppreference.com/w/cpp/string/basic_string/ends_with

#pragma region For String View

    /**
     * @brief Checks if the string view begins with the given prefix
     * @param[in] that The string view to find.
     * @param[in] sv A string view which may be a result of implicit conversion from \c std::basic_string.
     * @return True if the string view begins with the provided prefix, false otherwise.
    */
    template<class CharT, class Traits = std::char_traits<CharT>>
    bool starts_with(const std::basic_string_view<CharT, Traits>& that,
                     std::basic_string_view<CharT, Traits> sv) noexcept {
#if defined(YYCC_CPPFEAT_STARTS_ENDS_WITH)
        return that.starts_with(sv);
#else
        return std::basic_string_view<CharT, Traits>(that.data(), std::min(that.size(), sv.size()))
               == sv;
#endif
    }
    /**
     * @brief Checks if the string view begins with the given prefix
     * @param[in] that The string view to find.
     * @param[in] ch A single character.
     * @return True if the string view begins with the provided prefix, false otherwise.
    */
    template<class CharT, class Traits = std::char_traits<CharT>>
    bool starts_with(const std::basic_string_view<CharT, Traits>& that, CharT ch) noexcept {
#if defined(YYCC_CPPFEAT_STARTS_ENDS_WITH)
        return that.starts_with(ch);
#else
        return !that.empty() && Traits::eq(that.front(), ch);
#endif
    }
    /**
     * @brief Checks if the string view begins with the given prefix
     * @param[in] that The string view to find.
     * @param[in] s A null-terminated character string.
     * @return True if the string view begins with the provided prefix, false otherwise.
    */
    template<class CharT, class Traits = std::char_traits<CharT>>
    bool starts_with(const std::basic_string_view<CharT, Traits>& that, const CharT* s) noexcept {
#if defined(YYCC_CPPFEAT_STARTS_ENDS_WITH)
        return that.starts_with(s);
#else
        return starts_with(that, std::basic_string_view(s));
#endif
    }

    /**
     * @brief Checks if the string view ends with the given suffix
     * @param[in] that The string view to find.
     * @param[in] sv A string view which may be a result of implicit conversion from \c std::basic_string.
     * @return True if the string view ends with the provided suffix, false otherwise.
    */
    template<class CharT, class Traits = std::char_traits<CharT>>
    bool ends_with(const std::basic_string_view<CharT, Traits>& that,
                   std::basic_string_view<CharT, Traits> sv) noexcept {
#if defined(YYCC_CPPFEAT_STARTS_ENDS_WITH)
        return that.ends_with(sv);
#else
        return that.size() >= sv.size()
               && that.compare(that.size() - sv.size(),
                               std::basic_string_view<CharT, Traits>::npos,
                               sv)
                      == 0;
#endif
    }
    /**
     * @brief Checks if the string view ends with the given suffix
     * @param[in] that The string view to find.
     * @param[in] ch A single character.
     * @return True if the string view ends with the provided suffix, false otherwise.
    */
    template<class CharT, class Traits = std::char_traits<CharT>>
    bool ends_with(const std::basic_string_view<CharT, Traits>& that, CharT ch) noexcept {
#if defined(YYCC_CPPFEAT_STARTS_ENDS_WITH)
        return that.ends_with(ch);
#else
        return !that.empty() && Traits::eq(that.back(), ch);
#endif
    }
    /**
     * @brief Checks if the string view ends with the given suffix
     * @param[in] that The string view to find.
     * @param[in] s A null-terminated character string.
     * @return True if the string view ends with the provided suffix, false otherwise.
    */
    template<class CharT, class Traits = std::char_traits<CharT>>
    bool ends_with(const std::basic_string_view<CharT, Traits>& that, const CharT* s) noexcept {
#if defined(YYCC_CPPFEAT_STARTS_ENDS_WITH)
        return that.ends_with(s);
#else
        return ends_with(that, std::basic_string_view(s));
#endif
    }

#pragma endregion

#pragma region For String

    /**
     * @brief Checks if the string begins with the given prefix
     * @param[in] that The string to find.
     * @param[in] sv A string view which may be a result of implicit conversion from \c std::basic_string.
     * @return True if the string view begins with the provided prefix, false otherwise.
    */
    template<class CharT, class Traits = std::char_traits<CharT>>
    bool starts_with(const std::basic_string<CharT, Traits>& that,
                     std::basic_string_view<CharT, Traits> sv) noexcept {
#if defined(YYCC_CPPFEAT_STARTS_ENDS_WITH)
        return that.starts_with(sv);
#else
        return starts_with(std::basic_string_view<CharT, Traits>(that.data(), that.size()), sv);
#endif
    }
    /**
     * @brief Checks if the string begins with the given prefix
     * @param[in] that The string to find.
     * @param[in] ch A single character.
     * @return True if the string view begins with the provided prefix, false otherwise.
    */
    template<class CharT, class Traits = std::char_traits<CharT>>
    bool starts_with(const std::basic_string<CharT, Traits>& that, CharT ch) noexcept {
#if defined(YYCC_CPPFEAT_STARTS_ENDS_WITH)
        return that.starts_with(ch);
#else
        return starts_with(std::basic_string_view<CharT, Traits>(that.data(), that.size()), ch);
#endif
    }
    /**
     * @brief Checks if the string begins with the given prefix
     * @param[in] that The string to find.
     * @param[in] s A null-terminated character string.
     * @return True if the string view begins with the provided prefix, false otherwise.
    */
    template<class CharT, class Traits = std::char_traits<CharT>>
    bool starts_with(const std::basic_string<CharT, Traits>& that, const CharT* s) noexcept {
#if defined(YYCC_CPPFEAT_STARTS_ENDS_WITH)
        return that.starts_with(s);
#else
        return starts_with(std::basic_string_view<CharT, Traits>(that.data(), that.size()), s);
#endif
    }

    /**
     * @brief Checks if the string ends with the given suffix
     * @param[in] that The string to find.
     * @param[in] sv A string view which may be a result of implicit conversion from \c std::basic_string.
     * @return True if the string view ends with the provided suffix, false otherwise.
    */
    template<class CharT, class Traits = std::char_traits<CharT>>
    bool ends_with(const std::basic_string<CharT, Traits>& that,
                   std::basic_string_view<CharT, Traits> sv) noexcept {
#if defined(YYCC_CPPFEAT_STARTS_ENDS_WITH)
        return that.ends_with(sv);
#else
        return ends_with(std::basic_string_view<CharT, Traits>(that.data(), that.size()), sv);
#endif
    }
    /**
     * @brief Checks if the string ends with the given suffix
     * @param[in] that The string to find.
     * @param[in] ch A single character.
     * @return True if the string view ends with the provided suffix, false otherwise.
    */
    template<class CharT, class Traits = std::char_traits<CharT>>
    bool ends_with(const std::basic_string<CharT, Traits>& that, CharT ch) noexcept {
#if defined(YYCC_CPPFEAT_STARTS_ENDS_WITH)
        return that.ends_with(ch);
#else
        return ends_with(std::basic_string_view<CharT, Traits>(that.data(), that.size()), ch);
#endif
    }
    /**
     * @brief Checks if the string ends with the given suffix
     * @param[in] that The string to find.
     * @param[in] s A null-terminated character string.
     * @return True if the string view ends with the provided suffix, false otherwise.
    */
    template<class CharT, class Traits = std::char_traits<CharT>>
    bool ends_with(const std::basic_string<CharT, Traits>& that, const CharT* s) noexcept {
#if defined(YYCC_CPPFEAT_STARTS_ENDS_WITH)
        return that.ends_with(s);
#else
        return ends_with(std::basic_string_view<CharT, Traits>(that.data(), that.size()), s);
#endif
    }

#pragma endregion

} // namespace yycc::patch::starts_ends_with
