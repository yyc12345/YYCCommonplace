#pragma once
#include "../../macro/feature_probe.hpp"
#include "../../num/parse.hpp"
#include "../panic.hpp"
#include "../result.hpp"

#define NS_YYCC_STRING ::yycc::string
#define NS_YYCC_NUM_PARSE ::yycc::num::parse
#define NS_YYCC_RUST_RESULT ::yycc::rust::result

/**
 * @namespace yycc::rust::parse
 * @brief Provides Rust-inspired parsing utilities for converting strings to various types.
 * @details
 * This namespace contains template functions for parsing strings into different types
 * (floating-point, integral, boolean) with Rust-like Result error handling.
 */
namespace yycc::rust::num::parse {

#if defined(YYCC_CPPFEAT_EXPECTED)

    /// @brief The error type of parsing.
    using Error = NS_YYCC_NUM_PARSE::ParseError;

    /// @brief The result type of parsing.
    /// @tparam T The expected value type in result.
    template<typename T>
    using Result = NS_YYCC_RUST_RESULT::Result<T, Error>;

    /**
     * @brief Parses a string into a floating-point value.
     * @tparam T Floating-point type to parse into (float, double, etc.)
     * @param strl String view to parse
     * @param fmt Formatting flags for parsing (default: general)
     * @return Result<T> containing either the parsed value or an error
     */
    template<typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
    Result<T> parse(const NS_YYCC_STRING::u8string_view& strl,
            std::chars_format fmt = std::chars_format::general) {
        auto rv = NS_YYCC_NUM_PARSE::priv_parse<T>(strl, fmt);

        if (const auto* ptr = std::get_if<T>(&rv)) {
            return NS_YYCC_RUST_RESULT::Ok<Result<T>>(*ptr);
        } else if (const auto* ptr = std::get_if<Error>(&rv)) {
            return NS_YYCC_RUST_RESULT::Err<Result<T>>(*ptr);
        } else {
            // Unreachable
            RS_PANIC("unreachable code.");
        }
    }

    /**
     * @brief Parses a string into an integral value (excluding bool).
     * @tparam T Integral type to parse into (int, long, etc.)
     * @param strl String view to parse
     * @param base Numeric base for parsing (default: 10)
     * @return Result<T> containing either the parsed value or an error
     */
    template<typename T, std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, bool>, int> = 0>
    Result<T> parse(const NS_YYCC_STRING::u8string_view& strl, int base = 10) {
        auto rv = NS_YYCC_NUM_PARSE::priv_parse<T>(strl, base);

        if (const auto* ptr = std::get_if<T>(&rv)) {
            return NS_YYCC_RUST_RESULT::Ok<Result<T>>(*ptr);
        } else if (const auto* ptr = std::get_if<Error>(&rv)) {
            return NS_YYCC_RUST_RESULT::Err<Result<T>>(*ptr);
        } else {
            // Unreachable
            RS_PANIC("unreachable code.");
        }
    }

    /**
     * @brief Parses a string into a boolean value.
     * @tparam T Must be bool type
     * @param strl String view to parse
     * @return Result<bool> containing either the parsed value or an error
     */
    template<typename T, std::enable_if_t<std::is_same_v<T, bool>, int> = 0>
    Result<T> parse(const NS_YYCC_STRING::u8string_view& strl) {
        auto rv = NS_YYCC_NUM_PARSE::priv_parse<T>(strl);

        if (const auto* ptr = std::get_if<T>(&rv)) {
            return NS_YYCC_RUST_RESULT::Ok<Result<T>>(*ptr);
        } else if (const auto* ptr = std::get_if<Error>(&rv)) {
            return NS_YYCC_RUST_RESULT::Err<Result<T>>(*ptr);
        } else {
            // Unreachable
            RS_PANIC("unreachable code.");
        }
    }

#endif

}

#undef NS_YYCC_RUST_RESULT
#undef NS_YYCC_NUM_PARSE
#undef NS_YYCC_STRING
