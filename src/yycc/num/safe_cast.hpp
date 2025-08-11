#pragma once
#include <expected>
#include <concepts>
#include <limits>
#include <type_traits>

/**
 * @brief The namespace providing functions which safely cast numeric value from one type to another.
 * @details
 * When writing Rust code, I deeply realized that casting between types with different ranges is very important,
 * but it is greatly easy to make mistake which finally cause fatal error.
 * For widening conversions, we can safely perform them directly without consideration.
 * For narrowing conversions, we need to introduce a Result mechanism to determine if errors occur.
 * These features are implemented for all primitive types in Rust,
 * and managed uniformly through the \c From and \c TryFrom trait, which is perfect.
 * But in C++, we need to manually replicate them.
 *
 * In this namespace, we divide conversion functions into two categories:
 * \li \c to for definitely safe conversions,
 * \li \c try_to for potentially risky conversions.
 * There is a metaprogramming concept <TT>CAN_SAFE_TO\<T, U\></TT> to determine if a conversion is safe,
 * which applied for these functions as the constraint.
 * 
 * However, directly using \c CAN_SAFE_TO to determine whether the convertion is safe is incorrect.
 * In Rust, whether these conversions are safe is manually determined by different traits ( \c From and \c TryFrom).
 * But in C++, we brutally use the size of data types in compile-time to determine safe conversion,
 * which causes variable-length data types produces different \c CAN_SAFE_TO results on different platforms,
 * which affecting our code's portability.
 * For example, we can use 'to' directly for conversion between \c uint64_t and \c size_t: on 64-bit platforms,
 * but on 32-bit platforms it causes compile-time errors, resulting in portability issues.
 * 
 * Rust's solution is to define the minimum size of \c usize (32-bit), 
 * allowing safe conversion only for data smaller than this (e.g. \c u16 ).
 * But in C++ we can't do this because we can't know the minimum size of every variable-length primitive data type.
 * So we use another solution.
 *
 * Our solution is to enforce \c CAN_SAFE_TO rules for \c to functions but not for \c try_to functions.
 * Inside \c try_to, we use \c CAN_SAFE_TO to determine if safe conversion is possible.
 * If yes, convert directly, otherwise perform essential checks before convertion.
 * So under this solution, programmers need manually determine if conversion between two types is definitely safe before using \c to .
 * But at least, the compiler will throw errors when \c to is inviable, and the only thing you should do is switching to \c try_to.
 * Also, using \c try_to everywhere won't impact performance as unnecessary checks are removed via <TT>if constexpr</TT> statement in function.
 *
 * Currently unsupported conversions due to lack of demand:
 * \li Floating-point to floating-point conversions
 * \li Floating-point to integer conversions
 */
namespace yycc::num::safe_cast {

    /// @brief All possible error raised in this module.
    enum class CastError {
        Overflow,  ///< Overflow error occurred during conversion.
        Underflow, ///< Underflow error occurred during conversion.
    };

    /// @brief The result type in this module.
    template<typename T>
    using Result = std::expected<T, CastError>;

    /**
     * @private
     * @brief Check if an integer type can be safely converted to another integer type
     * @return True if it can be safely converted, false otherwise.
     */
    template<typename TDst, typename TSrc>
        requires std::integral<TDst> && std::integral<TSrc>
    constexpr bool can_safe_to() {
        // Fetch the sign info of TSrc and TDst.
        constexpr bool is_src_signed = std::is_signed_v<TSrc>;
        constexpr bool is_dst_signed = std::is_signed_v<TDst>;

        // Get the range of TSrc and TDst.
        constexpr TSrc src_min = std::numeric_limits<TSrc>::min();
        constexpr TSrc src_max = std::numeric_limits<TSrc>::max();
        constexpr TDst dst_min = std::numeric_limits<TDst>::min();
        constexpr TDst dst_max = std::numeric_limits<TDst>::max();

        if constexpr (is_src_signed) {
            if constexpr (is_dst_signed) {
                // Signed to signed conversion, both upper and lower bound need to be checked.
                // If completely within the range, it is definitely safe.
                return dst_min <= src_min && dst_max >= src_max;
            } else {
                // Signed to unsigned conversion, always unsafe.
                // Because negative numbers exist.
                return false;
            }
        } else {
            if constexpr (is_dst_signed) {
                // Unsigned to signed conversion, only check the upper bound.
                // If the upper bound is small enough, it is definitely safe.
                return dst_max >= src_max;
            } else {
                // Unsigned to unsigned conversion, only check the upper bound,
                // because the lower bound is 0.
                return dst_max >= src_max;
            }
        }
    }

    /**
     * @private
     * @brief Variable version of can_safe_to()
     * @details Convenience variable for subsequent constraints
     */
    template<typename TDst, typename TSrc>
        requires std::integral<TDst> && std::integral<TSrc>
    inline constexpr bool CAN_SAFE_TO = can_safe_to<TDst, TSrc>();

    /**
     * @brief Convert an integer type to another integer type.
     * @details Similar to Rust's \c From trait, but with reversed direction (from "from" to "to").
     * @return The converted result.
     */
    template<typename TDst, typename TSrc>
        requires std::integral<TDst> && std::integral<TSrc> && CAN_SAFE_TO<TDst, TSrc>
    TDst to(const TSrc& lhs) {
        return static_cast<TDst>(lhs);
    }

    /**
     * @brief Attempt to convert an integer type to another integer type.
     * @details Similar to Rust's \c TryFrom trait, but with reversed direction (from "from" to "to").
     * @return A result containing the conversion result or convertion error.
     */
    template<typename TDst, typename TSrc>
        requires std::integral<TDst> && std::integral<TSrc>
    Result<TDst> try_to(const TSrc& lhs) {
        // Check whether we can convert directly.
        if constexpr (CAN_SAFE_TO<TDst, TSrc>) {
            return static_cast<TDst>(lhs);
        } else {
            // Fetch the sign info of TSrc and TDst.
            constexpr bool is_src_signed = std::is_signed_v<TSrc>;
            constexpr bool is_dst_signed = std::is_signed_v<TDst>;

            // Fetch the range of TSrc and TDst.
            constexpr TSrc src_min = std::numeric_limits<TSrc>::min();
            constexpr TSrc src_max = std::numeric_limits<TSrc>::max();
            constexpr TDst dst_min = std::numeric_limits<TDst>::min();
            constexpr TDst dst_max = std::numeric_limits<TDst>::max();

            // Check whether we can convert safely.
            if constexpr (is_src_signed == is_dst_signed) {
                // If both are signed or unsigned, compare ranges directly.
                if (lhs < dst_min) return std::unexpected(CastError::Underflow);
                if (lhs > dst_max) return std::unexpected(CastError::Overflow);
                return static_cast<TDst>(lhs);
            } else {
                // If signs are different, we need to check the value range.
                if constexpr (is_src_signed) {
                    // If TSrc is signed, TDst is unsigned, we need to ensure lhs is not negative.
                    if (lhs < 0) return std::unexpected(CastError::Underflow);
                    if (lhs > dst_max) return std::unexpected(CastError::Overflow);
                    return static_cast<TDst>(lhs);
                } else {
                    // If TSrc is unsigned, TDst is signed, we need to ensure lhs is not greater than dst_max.
                    if (lhs > dst_max) return std::unexpected(CastError::Overflow);
                    return static_cast<TDst>(lhs);
                }
            }
        }
    }

} // namespace yycc::num::safe_cast
