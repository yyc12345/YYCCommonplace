#pragma once
#include "../string/reinterpret.hpp"
#include <string>
#include <array>
#include <type_traits>
#include <charconv>
#include <stdexcept>

#define NS_YYCC_STRING_REINTERPRET ::yycc::string::reinterpret

/**
 * @brief Provides stringify utilities for converting numeric and boolean values to strings.
 * @details
 * This namespace contains functions for stringifying various numeric types (integer, floating point)
 * and boolean values into string. It uses \c std::to_chars internally for efficient stringify.
 * @remarks
 * See https://en.cppreference.com/w/cpp/utility/to_chars for underlying called functions.
 * Default float precision = 6 is gotten from: https://en.cppreference.com/w/c/io/fprintf
 */
namespace yycc::num::stringify {

    /// @private
    /// @brief Size of the internal buffer used for string conversion.
    inline constexpr size_t STRINGIFY_BUFFER_SIZE = 64u;
    /// @private
    /// @brief Type alias for the buffer used in string conversion.
    using StringifyBuffer = std::array<char8_t, STRINGIFY_BUFFER_SIZE>;

    /**
     * @brief Return the string representation of given floating point value.
     * @tparam T The type derived from floating point type.
     * @param[in] num The value need to get string representation.
     * @param[in] fmt The floating point format used when getting string representation.
     * @param[in] precision The floating point precision used when getting string representation.
     * @return The string representation of given value.
    */
    template<typename T>
        requires(std::is_floating_point_v<T>)
    std::u8string stringify(T num, std::chars_format fmt = std::chars_format::general, int precision = 6) {
        namespace reinterpret = NS_YYCC_STRING_REINTERPRET;

        StringifyBuffer buffer;
        auto [ptr, ec] = std::to_chars(reinterpret::as_ordinary(buffer.data()),
                                       reinterpret::as_ordinary(buffer.data() + buffer.size()),
                                       num,
                                       fmt,
                                       precision);
        if (ec == std::errc()) {
            return std::u8string(buffer.data(), reinterpret::as_utf8(ptr) - buffer.data());
        } else if (ec == std::errc::value_too_large) {
            // Too short buffer. This should not happen.
            throw std::out_of_range("stringify() buffer is not sufficient.");
        } else {
            // Unreachable
            throw std::runtime_error("unreachable code.");
        }
    }
    /**
     * @brief Return the string representation of given integral value.
     * @tparam T The type derived from integral type except bool type.
     * @param[in] num The value need to get string representation.
     * @param[in] base Integer base used when getting string representation: a value between 2 and 36 (inclusive).
     * @return The string representation of given value.
    */
    template<typename T>
        requires(std::is_integral_v<T> && !std::is_same_v<T, bool>)
    std::u8string stringify(T num, int base = 10) {
        namespace reinterpret = NS_YYCC_STRING_REINTERPRET;

        StringifyBuffer buffer;
        auto [ptr, ec] = std::to_chars(reinterpret::as_ordinary(buffer.data()),
                                       reinterpret::as_ordinary(buffer.data() + buffer.size()),
                                       num,
                                       base);
        if (ec == std::errc()) {
            return std::u8string(buffer.data(), reinterpret::as_utf8(ptr) - buffer.data());
        } else if (ec == std::errc::value_too_large) {
            // Too short buffer. This should not happen.
            throw std::out_of_range("stringify() buffer is not sufficient.");
        } else {
            // Unreachable
            throw std::runtime_error("unreachable code.");
        }
    }
    /**
     * @brief Return the string representation of given bool value.
     * @tparam T The type derived from bool type.
     * @param[in] num The value need to get string representation.
     * @return The string representation of given value ("true" or "false").
    */
    template<typename T>
        requires(std::is_same_v<T, bool>)
    std::u8string stringify(T num) {
        if (num) return std::u8string(u8"true");
        else return std::u8string(u8"false");
    }

} // namespace yycc::num::stringify

#undef NS_YYCC_STRING_REINTERPRET
