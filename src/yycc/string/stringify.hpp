#pragma once
#include "../string.hpp"
#include "reinterpret.hpp"
#include <array>
#include <charconv>
#include <stdexcept>
#include <type_traits>

#define NS_YYCC_STRING ::yycc::string
#define NS_YYCC_STRING_REINTERPRET ::yycc::string::reinterpret

namespace yycc::string::stringify {

    // Developer Notes:
    // Reference: https://en.cppreference.com/w/cpp/utility/to_chars
    // Default float precision = 6 is gotten from: https://en.cppreference.com/w/c/io/fprintf

    inline constexpr size_t STRINGIFY_BUFFER_SIZE = 64u;
    using StringifyBuffer = std::array<NS_YYCC_STRING::u8char, STRINGIFY_BUFFER_SIZE>;

    /**
     * @brief Return the string representation of given floating point value.
     * @tparam T The type derived from floating point type.
     * @param[in] num The value need to get string representation.
     * @param[in] fmt The floating point format used when getting string representation.
     * @param[in] precision The floating point precision used when getting string representation.
     * @return The string representation of given value.
    */
    template<typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
    NS_YYCC_STRING::u8string stringify(T num,
                                       std::chars_format fmt = std::chars_format::general,
                                       int precision = 6) {
        namespace reinterpret = NS_YYCC_STRING_REINTERPRET;
        StringifyBuffer buffer;
        auto [ptr, ec] = std::to_chars(reinterpret::as_ordinary(buffer.data()),
                                       reinterpret::as_ordinary(buffer.data() + buffer.size()),
                                       num,
                                       fmt,
                                       precision);
        if (ec == std::errc()) {
            return NS_YYCC_STRING::u8string(buffer.data(),
                                            reinterpret::as_utf8(ptr) - buffer.data());
        } else if (ec == std::errc::value_too_large) {
            // Too short buffer. This should not happened
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
    template<typename T, std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, bool>, int> = 0>
    NS_YYCC_STRING::u8string stringify(T num, int base = 10) {
        namespace reinterpret = NS_YYCC_STRING_REINTERPRET;
        StringifyBuffer buffer;
        auto [ptr, ec] = std::to_chars(reinterpret::as_ordinary(buffer.data()),
                                       reinterpret::as_ordinary(buffer.data() + buffer.size()),
                                       num,
                                       base);
        if (ec == std::errc()) {
            return NS_YYCC_STRING::u8string(buffer.data(),
                                            reinterpret::as_utf8(ptr) - buffer.data());
        } else if (ec == std::errc::value_too_large) {
            // Too short buffer. This should not happened
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
    template<typename T, std::enable_if_t<std::is_same_v<T, bool>, int> = 0>
    NS_YYCC_STRING::u8string stringify(T num) {
        if (num) return NS_YYCC_STRING::u8string(YYCC_U8("true"));
        else return NS_YYCC_STRING::u8string(YYCC_U8("false"));
    }

} // namespace yycc::string::stringify

#undef NS_YYCC_STRING_REINTERPRET
#undef NS_YYCC_STRING
