#pragma once
#include "../string/op.hpp"
#include "../string/reinterpret.hpp"
#include <string_view>
#include <type_traits>
#include <charconv>
#include <stdexcept>
#include <expected>

#define NS_YYCC_STRING_REINTERPRET ::yycc::string::reinterpret
#define NS_YYCC_STRING_OP ::yycc::string::op

/**
 * @brief Provides string parsing utilities for converting strings to numeric and boolean values.
 * @details
 * This namespace contains functions for parsing strings into various numeric types (integer, floating point)
 * and boolean values. It uses \c std::from_chars internally for efficient parsing.
 * @remarks See https://zh.cppreference.com/w/cpp/utility/from_chars for underlying called functions.
 */
namespace yycc::num::parse {

    /// @brief The error kind when parsing string into number.
    enum class ParseError {
        PartiallyParsed, ///< Only a part of given string was parsed. The whole string may be invalid.
        InvalidString,   ///< Given string is a invalid number string.
        OutOfRange,      ///< Given string is valid but its value out of the range of given number type.
    };

    /// @brief The return value of internal parse function which ape `std::expected`.
    template<typename T>
    using ParseResult = std::expected<T, ParseError>;

    /**
     * @private
     * @brief Internal parsing function for floating point types
     * @tparam T Floating point type (float, double, etc)
     * @param strl The UTF-8 string view to parse
     * @param fmt The floating point format to use
     * @return ParseResult<T> containing either the parsed value or a ParseError
     */
    template<typename T>
        requires(std::is_floating_point_v<T>)
    ParseResult<T> parse(const std::u8string_view& strl, std::chars_format fmt) {
        namespace reinterpret = NS_YYCC_STRING_REINTERPRET;

        T rv;
        const auto* head = reinterpret::as_ordinary(strl.data());
        const auto* tail = reinterpret::as_ordinary(strl.data() + strl.size());
        auto [ptr, ec] = std::from_chars(head, tail, rv, fmt);

        if (ec == std::errc()) {
            // Parse completely.
            // But we need to check whether the whole string was parsed.
            if (ptr == tail) return rv;
            else return std::unexpected(ParseError::PartiallyParsed);
        } else if (ec == std::errc::invalid_argument) {
            // Given string is invalid
            return std::unexpected(ParseError::InvalidString);
        } else if (ec == std::errc::result_out_of_range) {
            // Given string is out of range
            return std::unexpected(ParseError::OutOfRange);
        } else {
            // Unreachable
            throw std::runtime_error("invalid ec.");
        }
    }

    /**
     * @private
     * @brief Internal parsing function for integral types (except bool)
     * @tparam T Integral type (int, long, etc)
     * @param strl The UTF-8 string view to parse
     * @param base Numeric base (2-36)
     * @return ParseResult<T> containing either the parsed value or a ParseError
     */
    template<typename T>
        requires(std::is_integral_v<T> && !std::is_same_v<T, bool>)
    ParseResult<T> parse(const std::u8string_view& strl, int base) {
        namespace reinterpret = NS_YYCC_STRING_REINTERPRET;

        T rv;
        const auto* head = reinterpret::as_ordinary(strl.data());
        const auto* tail = reinterpret::as_ordinary(strl.data() + strl.size());
        auto [ptr, ec] = std::from_chars(head, tail, rv, base);

        if (ec == std::errc()) {
            // Parse completely.
            // But we need to check whether the whole string was parsed.
            if (ptr == tail) return rv;
            else return std::unexpected(ParseError::PartiallyParsed);
        } else if (ec == std::errc::invalid_argument) {
            // Given string is invalid
            return std::unexpected(ParseError::InvalidString);
        } else if (ec == std::errc::result_out_of_range) {
            // Given string is out of range
            return std::unexpected(ParseError::OutOfRange);
        } else {
            // Unreachable
            throw std::runtime_error("invalid ec.");
        }
    }

    /**
     * @private
     * @brief Internal parsing function for boolean type
     * @tparam T Must be bool type
     * @param strl The UTF-8 string view to parse ("true" or "false", case insensitive)
     * @return ParseResult<bool> containing either the parsed value or a ParseError
     */
    template<typename T>
        requires(std::is_same_v<T, bool>)
    ParseResult<T> parse(const std::u8string_view& strl) {
        // Get lower case
        auto lower_case = NS_YYCC_STRING_OP::to_lower(strl);
        // Compare result
        if (lower_case == u8"true") return true;
        else if (lower_case == u8"false") return false;
        else return ParseError::InvalidString;
    }

} // namespace yycc::num::parse

#undef NS_YYCC_STRING_OP
#undef NS_YYCC_STRING_REINTERPRET
