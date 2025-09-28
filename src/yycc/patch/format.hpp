/**
 * @file
 * @brief The patch for \c std::format to allow UTF8 string as arguments.
 * @details
 * As we known, \c std::format only allow \c char and \c wchar_t as its char type in C++ 23 currently.
 * So it is impossible to use UTF8 string for std::format, both format string and argument.
 * This namespace give a patch for this shortcoming.
 * By including this file directly, you will have abilities that use UTF8 string as argument in \c std::format with \c char char type.
*/
#pragma once
#include "../string/reinterpret.hpp"
#include <format>
#include <string>
#include <string_view>

#define NS_YYCC_STRING_REINTERPRET ::yycc::string::reinterpret

#pragma region Utf8 Format

namespace yycc::patch::format {

    template<class... Args>
    std::string format(std::format_string<Args...> fmt, Args&&... args) {
        return std::vformat(fmt.get(), std::make_format_args(args...));
    }

    template<class... Args>
    std::u8string format(const std::u8string_view& fmt, Args&&... args) {
        return NS_YYCC_STRING_REINTERPRET::as_utf8(
            std::vformat(NS_YYCC_STRING_REINTERPRET::as_ordinary_view(fmt), std::make_format_args(args...)));
    }

} // namespace yycc::patch::format

#pragma endregion

#pragma region Utf8 Formatter

// Add std::formatter specialization for "char8_t"
template<>
struct std::formatter<char8_t, char> {
    constexpr auto parse(auto& ctx) { return underlying_formatter.parse(ctx); }
    auto format(const char8_t& str, auto& ctx) const { return underlying_formatter.format(NS_YYCC_STRING_REINTERPRET::as_ordinary(str), ctx); }

private:
    std::formatter<char, char> underlying_formatter{};
};

// Add std::formatter specialization for "char8_t*"
template<>
struct std::formatter<char8_t*, char> {
    constexpr auto parse(auto& ctx) { return underlying_formatter.parse(ctx); }
    auto format(char8_t* str, auto& ctx) const { return underlying_formatter.format(NS_YYCC_STRING_REINTERPRET::as_ordinary(str), ctx); }

private:
    std::formatter<const char*, char> underlying_formatter{};
};

// Add std::formatter specialization for "const char8_t*"
template<>
struct std::formatter<const char8_t*, char> {
    constexpr auto parse(auto& ctx) { return underlying_formatter.parse(ctx); }
    auto format(const char8_t* str, auto& ctx) const {
        return underlying_formatter.format(NS_YYCC_STRING_REINTERPRET::as_ordinary(str), ctx);
    }

private:
    std::formatter<const char*, char> underlying_formatter{};
};

// Add std::formatter specialization for "char8_t[N]"
template<std::size_t N>
struct std::formatter<char8_t[N], char> {
    constexpr auto parse(auto& ctx) { return underlying_formatter.parse(ctx); }
    auto format(const char8_t (&str)[N], auto& ctx) const {
        return underlying_formatter.format(std::basic_string_view<char>(NS_YYCC_STRING_REINTERPRET::as_ordinary(str), N - 1), ctx);
    }

private:
    std::formatter<std::basic_string_view<char>, char> underlying_formatter{};
};

// Add std::formatter specialization for "std::u8string"
template<class Traits, class Alloc>
struct std::formatter<std::basic_string<char8_t, Traits, Alloc>, char> {
    constexpr auto parse(auto& ctx) { return underlying_formatter.parse(ctx); }
    auto format(const std::u8string& str, auto& ctx) const {
        return underlying_formatter.format(NS_YYCC_STRING_REINTERPRET::as_ordinary_view(str), ctx);
    }

private:
    std::formatter<std::basic_string_view<char>, char> underlying_formatter{};
};

// Add std::formatter specialization for "std::u8string_view"
template<class Traits>
struct std::formatter<std::basic_string_view<char8_t, Traits>, char> {
    constexpr auto parse(auto& ctx) { return underlying_formatter.parse(ctx); }
    auto format(std::u8string_view sv, auto& ctx) const {
        return underlying_formatter.format(NS_YYCC_STRING_REINTERPRET::as_ordinary_view(sv), ctx);
    }

private:
    std::formatter<std::basic_string_view<char>, char> underlying_formatter{};
};

#pragma endregion

#undef NS_YYCC_STRING_REINTERPRET
