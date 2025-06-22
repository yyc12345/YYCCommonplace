#pragma once
#include "../string.hpp"
#include "op.hpp"
#include "reinterpret.hpp"
#include <charconv>
#include <stdexcept>
#include <type_traits>
#include <variant>

#define NS_YYCC_STRING ::yycc::string
#define NS_YYCC_STRING_REINTERPRET ::yycc::string::reinterpret
#define NS_YYCC_STRING_OP ::yycc::string::op

namespace yycc::string::parse {

    // Developer Notes:
    // Reference: https://zh.cppreference.com/w/cpp/utility/from_chars

    /// @brief The error kind when parsing string into number.
    enum class ParseError {
        PartiallyParsed, ///< Only a part of given string was parsed. The whole string may be invalid.
        InvalidString,   ///< Given string is a invalid number string.
        OutOfRange, ///< Given string is valid but its value out of the range of given number type.
    };

    template<typename T, std::enable_if_t<!std::is_same_v<T, ParseError>, int> = 0>
    using ParseResult = std::variant<T, ParseError>;

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
    ParseResult<T> _priv_parse(const NS_YYCC_STRING::u8string_view& strl, std::chars_format fmt) {
        namespace reinterpret = NS_YYCC_STRING_REINTERPRET;

        T rv;
        const auto* head = reinterpret::as_ordinary(strl.data());
        const auto* tail = reinterpret::as_ordinary(strl.data() + strl.size());
        auto [ptr, ec] = std::from_chars(head, tail, rv, fmt);

        if (ec == std::errc()) {
            // Parse completely.
            // But we need to check whether the whole string was parsed.
            if (ptr == tail) return rv;
            else return ParseError::PartiallyParsed;
        } else if (ec == std::errc::invalid_argument) {
            // Given string is invalid
            return ParseError::InvalidString;
        } else if (ec == std::errc::result_out_of_range) {
            // Given string is out of range
            return ParseError::OutOfRange;
        } else {
            // Unreachable
            throw std::runtime_error("invalid ec.");
        }
    }

    template<typename T, std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, bool>, int> = 0>
    ParseResult<T> _priv_parse(const NS_YYCC_STRING::u8string_view& strl, int base) {
        namespace reinterpret = NS_YYCC_STRING_REINTERPRET;

        T rv;
        const auto* head = reinterpret::as_ordinary(strl.data());
        const auto* tail = reinterpret::as_ordinary(strl.data() + strl.size());
        auto [ptr, ec] = std::from_chars(head, tail, rv, base);

        if (ec == std::errc()) {
            // Parse completely.
            // But we need to check whether the whole string was parsed.
            if (ptr == tail) return rv;
            else return ParseError::PartiallyParsed;
        } else if (ec == std::errc::invalid_argument) {
            // Given string is invalid
            return ParseError::InvalidString;
        } else if (ec == std::errc::result_out_of_range) {
            // Given string is out of range
            return ParseError::OutOfRange;
        } else {
            // Unreachable
            throw std::runtime_error("invalid ec.");
        }
    }

    template<typename T, std::enable_if_t<std::is_same_v<T, bool>, int> = 0>
    ParseResult<T> _priv_parse(const NS_YYCC_STRING::u8string_view& strl) {
        // Get lower case
        auto lower_case = NS_YYCC_STRING_OP::to_lower(strl);
        // Compare result
        if (strl == YYCC_U8("true")) return true;
        else if (strl == YYCC_U8("false")) return false;
        else return ParseError::InvalidString;
    }

    /**
     * @brief Try parsing given string to floating point types.
     * @tparam T The type derived from floating point type.
     * @param[in] strl The string need to be parsed.
     * @param[out] num
     * The variable receiving result.
     * There is no guarantee that the content is not modified when parsing failed.
     * @param[in] fmt The floating point format used when try parsing.
     * @return True if success, otherwise false.
    */
    template<typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
    bool try_parse(const NS_YYCC_STRING::u8string_view& strl,
                   T& num,
                   std::chars_format fmt = std::chars_format::general) {
        auto rv = _priv_parse<T>(strl, fmt);
        if (const auto* ptr = std::get_if<T>(rv)) {
            num = *ptr;
            return true;
        } else if (const auto* ptr = std::get_if<ParseError>(rv)) {
            return false;
        } else {
            // Unreachable
            throw std::runtime_error("unreachable code.");
        }
    }
    /**
     * @brief Try parsing given string to integral types.
     * @tparam T The type derived from integral type except bool type.
     * @param[in] strl The string need to be parsed.
     * @param[out] num
     * The variable receiving result.
     * There is no guarantee that the content is not modified when parsing failed.
     * @param[in] base Integer base to use: a value between 2 and 36 (inclusive).
     * @return True if success, otherwise false.
    */
    template<typename T, std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, bool>, int> = 0>
    bool try_parse(const NS_YYCC_STRING::u8string_view& strl, T& num, int base = 10) {
        auto rv = _priv_parse<T>(strl, base);
        if (const auto* ptr = std::get_if<T>(rv)) {
            num = *ptr;
            return true;
        } else if (const auto* ptr = std::get_if<ParseError>(rv)) {
            return false;
        } else {
            // Unreachable
            throw std::runtime_error("unreachable code.");
        }
    }
    /**
     * @brief Try parsing given string to bool types.
     * @tparam T The type derived from bool type.
     * @param[in] strl The string need to be parsed ("true" or "false", case insensitive).
     * @param[out] num
     * The variable receiving result.
     * There is no guarantee that the content is not modified when parsing failed.
     * @return True if success, otherwise false.
    */
    template<typename T, std::enable_if_t<std::is_same_v<T, bool>, int> = 0>
    bool try_parse(const NS_YYCC_STRING::u8string_view& strl, T& num) {
        auto rv = _priv_parse<T>(strl);
        if (const auto* ptr = std::get_if<T>(rv)) {
            num = *ptr;
            return true;
        } else if (const auto* ptr = std::get_if<ParseError>(rv)) {
            return false;
        } else {
            // Unreachable
            throw std::runtime_error("unreachable code.");
        }
    }

    /**
     * @brief Parse given string to floating point types.
     * @tparam T The type derived from floating point type.
     * @param[in] strl The string need to be parsed.
     * @param[in] fmt The floating point format used when try parsing.
     * @return
     * The parsing result.
     * There is no guarantee about the content of this return value when parsing failed.
     * It may be any possible value but usually is its default value.
     * @exception std::invalid_argument Can not parse given string.
    */
    template<typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
    T parse(const NS_YYCC_STRING::u8string_view& strl,
            std::chars_format fmt = std::chars_format::general) {
        T rv;
        if (try_parse(strl, rv, fmt)) return rv;
        else throw std::invalid_argument("can not parse given string");
    }
    /**
     * @brief Parse given string to integral type types.
     * @tparam T The type derived from integral type except bool type.
     * @param[in] strl The string need to be parsed.
     * @param[in] base Integer base to use: a value between 2 and 36 (inclusive).
     * @return
     * The parsing result.
     * There is no guarantee about the content of this return value when parsing failed.
     * It may be any possible value but usually is its default value.
     * @exception std::invalid_argument Can not parse given string.
    */
    template<typename T, std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, bool>, int> = 0>
    T parse(const NS_YYCC_STRING::u8string_view& strl, int base = 10) {
        T rv;
        if (try_parse(strl, rv, base)) return rv;
        else throw std::invalid_argument("can not parse given string");
    }
    /**
     * @brief Parse given string to bool types.
     * @tparam T The type derived from bool type.
     * @param[in] strl The string need to be parsed ("true" or "false", case insensitive).
     * @return
     * The parsing result.
     * There is no guarantee about the content of this return value when parsing failed.
     * It may be any possible value but usually is its default value.
     * @exception std::invalid_argument Can not parse given string.
    */
    template<typename T, std::enable_if_t<std::is_same_v<T, bool>, int> = 0>
    T parse(const NS_YYCC_STRING::u8string_view& strl) {
        T rv;
        if (try_parse(strl, rv)) return rv;
        else throw std::invalid_argument("can not parse given string");
    }

} // namespace yycc::string::parse

#undef NS_YYCC_STRING_OP
#undef NS_YYCC_STRING_REINTERPRET
#undef NS_YYCC_STRING
