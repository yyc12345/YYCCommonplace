#pragma once
#include "../macro/os_detector.hpp"
#include "../macro/compiler_detector.hpp"
#include <stdexcept>
#include <optional>
#include <concepts>
#include <limits>

// Choose the function family for hardware based overflow.
#if defined(YYCC_CC_GCC) || defined(YYCC_CC_CLANG)
#define YYCC_HARDWARE_OVERFLOW_GCC_FNS
#elif defined(YYCC_OS_WINDOWS)
#define YYCC_HARDWARE_OVERFLOW_WIN32_FNS
#else
#error "Not supported platform or compiler for integral overflow function family."
#endif

// Import essential header if we are using Windows function family.
#if defined(YYCC_HARDWARE_OVERFLOW_WIN32_FNS)
#include "../windows/import_guard_head.hpp"
#include <intsafe.h>
#include "../windows/import_guard_tail.hpp"
#endif

/**
 * @brief The namespace providing Rust-like safe arithmetic operations.
 * @details
 * After writing some programs in Rust, I've deeply realized the richness of operators for primitive types in Rust.
 * You can explicitly specify the behavior of arithmetic overflow
 * (choose one of wrapping, checked, overflowing, and saturating).
 * Therefore, I'm replicating these convenient features from Rust in this namespace.
 * 
 * Additionally, I provide a bunch of extra operations, called ordinary operation.
 * These functions are just an alias to wrapping operator, indicating wrapping is the normal case.
 * These normal operators will not have any undefined behavior which C++ will make.
 * It basically like \c Add, \c Sub, \c Mul, and \c Div traits in Rust,
 * providing safe operations for primitive types.
 */
namespace yycc::num::safe_op {

    /*
    Implementation notes:

    - Wrapping operation:
        - Unsigned integer use default overflow behavior.
        - Signed integer will be casted into unsigned integer before operation to simulate wrapping overflow.
    - Checked operation:
        - Use compiler built-in function to detect overflow, return std::optional<T>.
        - Return std::nullopt when division by zero or overflow.
    - Overflowing operation:
        - Return std::pair<T, bool> where bool indicates whether overflow occurs.
        - Explicitly handle 2 division undefined behavior:
            - Division by zero.
            - Signed minimum value divided by -1.
    - Saturating operation:
        - Return maximum or minimum value when overflow or underflow occurs.
        - Determine saturation direction (overflow or underflow) based on operand sign.
    */

#pragma region Undefined Behaviors

    /*
    YYC MARK:
    Following undefined behaviors should be noticed:
    - Signed integer overflow and underflow (e.g. INT_MAX + 1).
    - Perform `INT_MIN / -1` division.
    - The right operand in division is zero.
    */

    /**
     * @private
     * @brief Adds two numbers while considering the undefined behavior of signed integer overflow.
     * @tparam T Integer type
     * @param[in] a The left operand of the addition
     * @param[in] b The right operand of the addition
     * @return The result of the addition that takes into account the undefined behavior of signed integers.
     */
    template<typename T>
        requires std::integral<T>
    T ub_signed_int_add(T a, T b) {
        if constexpr (std::is_unsigned_v<T>) {
            // Add, Sub, Mul and Div for unsigned integer is natural wrapping.
            // So we can use operator simply and directly.
            return a + b;
        } else {
            // In C++, it is undefined behavior that signed integer overflow.
            // So we need cast them into unsigned integer forcely before operation,
            // do operation for them, and cast the result back to simulate wrapping overflow.
            using UT = std::make_unsigned_t<T>;
            return static_cast<T>(static_cast<UT>(a) + static_cast<UT>(b));
        }
    }

    /**
     * @private
     * @brief Subtracts two numbers while considering the undefined behavior of signed integer overflow.
     * @tparam T Integer type
     * @param[in] a The left operand of the subtraction
     * @param[in] b The right operand of the subtraction
     * @return The result of the subtraction that takes into account the undefined behavior of signed integers.
     */
    template<typename T>
        requires std::integral<T>
    T ub_signed_int_sub(T a, T b) {
        if constexpr (std::is_unsigned_v<T>) {
            return a - b;
        } else {
            using UT = std::make_unsigned_t<T>;
            return static_cast<T>(static_cast<UT>(a) - static_cast<UT>(b));
        }
    }

    /**
     * @private
     * @brief Multiplies two numbers while considering the undefined behavior of signed integer overflow.
     * @tparam T Integer type
     * @param[in] a The left operand of the multiplication
     * @param[in] b The right operand of the multiplication
     * @return The result of the multiplication that takes into account the undefined behavior of signed integers.
     */
    template<typename T>
        requires std::integral<T>
    T ub_signed_int_mul(T a, T b) {
        if constexpr (std::is_unsigned_v<T>) {
            return a * b;
        } else {
            using UT = std::make_unsigned_t<T>;
            return static_cast<T>(static_cast<UT>(a) * static_cast<UT>(b));
        }
    }

    /**
     * @private
     * @brief Checks for undefined behavior when dividing the minimum signed integer value by -1.
     * @tparam T Integer type
     * @param[in] a The left operand of the division
     * @param[in] b The right operand of the division
     * @return Returns true if undefined behavior will occur, false otherwise.
     */
    template<typename T>
        requires std::integral<T>
    bool ub_signed_int_min_div_minus_one(T a, T b) {
        if constexpr (std::is_signed_v<T>) {
            // For signed value, `INT_MIN / -1` may cause overflow,
            // which finally cause the undefined behavior,
            // due to the truth that `INT_MIN == -INT_MIN - 1`.
            if (b == -1 && a == std::numeric_limits<T>::min()) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    /**
     * @private
     * @brief Checks for the undefined behavior of division by zero.
     * @tparam T Integer type
     * @param[in] a The left operand of the division
     * @param[in] b The right operand of the division
     * @return Returns true if undefined behavior will occur, false otherwise.
     */
    template<typename T>
        requires std::integral<T>
    bool ub_div_zero([[maybe_unused]] T a, T b) {
        return b == 0;
    }

#pragma endregion

#pragma region Hardware Operation Overflow

// YYC MARK:
// If we are using Windows function family,
// we define a convenient macro assisting overflow calculation.
#if defined(YYCC_HARDWARE_OVERFLOW_WIN32_FNS)
#define WIN_EASY_OPER(fn, ty, a, b, c) FAILED(fn(static_cast<ty>(a), static_cast<ty>(b), reinterpret_cast<ty*>(c)))
#endif

    /**
     * @private
     * @brief Addition with overflow detection based on hardware instructions.
     * @param[in] a The left operand of the addition.
     * @param[in] b The right operand of the addition.
     * @param[out] c The pointer to the variable storing result.
     * @return Returns true if an overflow occurs, false otherwise.
     */
    template<typename T>
        requires std::integral<T>
    bool hardware_add_overflow(T a, T b, T* c) {
        if (c == nullptr) [[unlikely]]
            throw std::logic_error("invalid nullptr");
#if defined(YYCC_HARDWARE_OVERFLOW_GCC_FNS)
        return __builtin_add_overflow(a, b, c);
#else
        bool overflow = false;
        constexpr size_t T_SIZE = sizeof(T);
        if constexpr (std::is_signed_v<T>) {
            if constexpr (T_SIZE == 8) {
                overflow = WIN_EASY_OPER(LongLongAdd, LONGLONG, a, b, c);
            } else if constexpr (T_SIZE == 4) {
                overflow = WIN_EASY_OPER(LongAdd, LONG, a, b, c);
            } else if constexpr (T_SIZE == 2) {
                overflow = WIN_EASY_OPER(ShortAdd, SHORT, a, b, c);
            } else if constexpr (T_SIZE == 1) {
                overflow = WIN_EASY_OPER(Int8Add, INT8, a, b, c);
            } else {
                static_assert(std::false_type::value, "not supported integral type.");
            }
        } else {
            if constexpr (T_SIZE == 8) {
                overflow = WIN_EASY_OPER(ULongLongAdd, ULONGLONG, a, b, c);
            } else if constexpr (T_SIZE == 4) {
                overflow = WIN_EASY_OPER(ULongAdd, ULONG, a, b, c);
            } else if constexpr (T_SIZE == 2) {
                overflow = WIN_EASY_OPER(UShortAdd, USHORT, a, b, c);
            } else if constexpr (T_SIZE == 1) {
                overflow = WIN_EASY_OPER(UInt8Add, UINT8, a, b, c);
            } else {
                static_assert(std::false_type::value, "not supported integral type.");
            }
        }
        // Due to the limitation of Windows function family,
        // if overflow or underflow occurs, there is no calculation result.
        // So we need fill the wrapping value manually.
        if (overflow) *c = ub_signed_int_add<T>(a, b);
        return overflow;
#endif
    }

    /**
     * @private
     * @brief Subtraction with overflow detection based on hardware instructions.
     * @param[in] a The left operand of the subtraction.
     * @param[in] b The right operand of the subtraction.
     * @param[out] c The pointer to the variable storing the result.
     * @return Returns true if an overflow occurs, false otherwise.
     */
    template<typename T>
        requires std::integral<T>
    bool hardware_sub_overflow(T a, T b, T* c) {
        if (c == nullptr) [[unlikely]]
            throw std::logic_error("invalid nullptr");
#if defined(YYCC_HARDWARE_OVERFLOW_GCC_FNS)
        return __builtin_sub_overflow(a, b, c);
#else
        bool overflow = false;
        constexpr size_t T_SIZE = sizeof(T);
        if constexpr (std::is_signed_v<T>) {
            if constexpr (T_SIZE == 8) {
                overflow = WIN_EASY_OPER(LongLongSub, LONGLONG, a, b, c);
            } else if constexpr (T_SIZE == 4) {
                overflow = WIN_EASY_OPER(LongSub, LONG, a, b, c);
            } else if constexpr (T_SIZE == 2) {
                overflow = WIN_EASY_OPER(ShortSub, SHORT, a, b, c);
            } else if constexpr (T_SIZE == 1) {
                overflow = WIN_EASY_OPER(Int8Sub, INT8, a, b, c);
            } else {
                static_assert(std::false_type::value, "not supported integral type.");
            }
        } else {
            if constexpr (T_SIZE == 8) {
                overflow = WIN_EASY_OPER(ULongLongSub, ULONGLONG, a, b, c);
            } else if constexpr (T_SIZE == 4) {
                overflow = WIN_EASY_OPER(ULongSub, ULONG, a, b, c);
            } else if constexpr (T_SIZE == 2) {
                overflow = WIN_EASY_OPER(UShortSub, USHORT, a, b, c);
            } else if constexpr (T_SIZE == 1) {
                overflow = WIN_EASY_OPER(UInt8Sub, UINT8, a, b, c);
            } else {
                static_assert(std::false_type::value, "not supported integral type.");
            }
        }
        // Similarly, manually calculate wrapping value.
        if (overflow) *c = ub_signed_int_sub<T>(a, b);
        return overflow;
#endif
    }

    /**
     * @private
     * @brief Multiplication with overflow detection based on hardware instructions.
     * @param[in] a The left operand of the multiplication.
     * @param[in] b The right operand of the multiplication.
     * @param[out] c The reference to the variable storing the result.
     * @return Returns true if an overflow occurs, false otherwise.
     */
    template<typename T>
        requires std::integral<T>
    bool hardware_mul_overflow(T a, T b, T* c) {
        if (c == nullptr) [[unlikely]]
            throw std::logic_error("invalid nullptr");
#if defined(YYCC_HARDWARE_OVERFLOW_GCC_FNS)
        return __builtin_mul_overflow(a, b, c);
#else
        bool overflow = false;
        constexpr size_t T_SIZE = sizeof(T);
        if constexpr (std::is_signed_v<T>) {
            if constexpr (T_SIZE == 8) {
                overflow = WIN_EASY_OPER(LongLongMult, LONGLONG, a, b, c);
            } else if constexpr (T_SIZE == 4) {
                overflow = WIN_EASY_OPER(LongMult, LONG, a, b, c);
            } else if constexpr (T_SIZE == 2) {
                overflow = WIN_EASY_OPER(ShortMult, SHORT, a, b, c);
            } else if constexpr (T_SIZE == 1) {
                overflow = WIN_EASY_OPER(Int8Mult, INT8, a, b, c);
            } else {
                static_assert(std::false_type::value, "not supported integral type.");
            }
        } else {
            if constexpr (T_SIZE == 8) {
                overflow = WIN_EASY_OPER(ULongLongMult, ULONGLONG, a, b, c);
            } else if constexpr (T_SIZE == 4) {
                overflow = WIN_EASY_OPER(ULongMult, ULONG, a, b, c);
            } else if constexpr (T_SIZE == 2) {
                overflow = WIN_EASY_OPER(UShortMult, USHORT, a, b, c);
            } else if constexpr (T_SIZE == 1) {
                overflow = WIN_EASY_OPER(UInt8Mult, UINT8, a, b, c);
            } else {
                static_assert(std::false_type::value, "not supported integral type.");
            }
        }
        // Similarly, manually calculate wrapping value.
        if (overflow) *c = ub_signed_int_mul<T>(a, b);
        return overflow;
#endif
    }

// YYC MARK:
// Delete the defined macro to prevent polluting the content later.
#if defined(YYCC_HARDWARE_OVERFLOW_WIN32_FNS)
#undef WIN_EASY_OPER
#endif

#pragma endregion

#pragma region Wrapping operations

    // YYC MARK:
    // wrapping_* function family will wrap the result in any scenario.

    /**
     * @brief Performs a wrapping addition operation on two integers.
     * @tparam T Integer type.
     * @param[in] a The left operand of the addition.
     * @param[in] b The right operand of the addition.
     * @return The wrapping result of the addition operation.
     */
    template<typename T>
        requires std::integral<T>
    T wrapping_add(T a, T b) {
        return ub_signed_int_add(a, b);
    }

    /**
     * @brief Performs a wrapping subtraction operation on two integers.
     * @tparam T Integer type.
     * @param[in] a The left operand of the subtraction.
     * @param[in] b The right operand of the subtraction.
     * @return The wrapping result of the subtraction operation.
     */
    template<typename T>
        requires std::integral<T>
    T wrapping_sub(T a, T b) {
        return ub_signed_int_sub(a, b);
    }

    /**
     * @brief Performs a wrapping multiplication operation on two integers.
     * @tparam T Integer type.
     * @param[in] a The left operand of the multiplication.
     * @param[in] b The right operand of the multiplication.
     * @return The wrapping result of the multiplication operation.
     */
    template<typename T>
        requires std::integral<T>
    T wrapping_mul(T a, T b) {
        return ub_signed_int_mul(a, b);
    }

    /**
     * @brief Performs a wrapping division operation on two integers.
     * @tparam T Integer type.
     * @param[in] a The left operand of the division.
     * @param[in] b The right operand of the division.
     * @return The wrapping result of the division operation.
     * @exception std::logic_error If division by zero occurs.
     */
    template<typename T>
        requires std::integral<T>
    T wrapping_div(T a, T b) {
        // Division by zero is undefined behavior.
        if (ub_div_zero(a, b)) throw std::logic_error("div with 0");
        // `INT_MIN / -1` overflow undefined behavior.
        if (ub_signed_int_min_div_minus_one(a, b))
            // "a" self is the minimum value of signed integer, return it directly.
            // There is no need to re-fetch it by std::numeric_limits.
            return a;
        return a / b;
    }

#pragma endregion

#pragma region Checked operations

    // YYC MARK:
    // If overflow occurs when using checked_* function family,
    // these functions will return std::nullopt, otherwise the computed result.

    /**
     * @brief Performs a checked addition operation on two integers.
     * @tparam T Integer type.
     * @param[in] a The left operand of the addition.
     * @param[in] b The right operand of the addition.
     * @return An std::optional containing the result if no overflow occurs, otherwise std::nullopt.
     */
    template<typename T>
        requires std::integral<T>
    std::optional<T> checked_add(T a, T b) {
        T result;
        if (hardware_add_overflow(a, b, &result)) return std::nullopt;
        return result;
    }

    /**
     * @brief Performs a checked subtraction operation on two integers.
     * @tparam T Integer type.
     * @param[in] a The left operand of the subtraction.
     * @param[in] b The right operand of the subtraction.
     * @return An std::optional containing the result if no overflow occurs, otherwise std::nullopt.
     */
    template<typename T>
        requires std::integral<T>
    std::optional<T> checked_sub(T a, T b) {
        T result;
        if (hardware_sub_overflow(a, b, &result)) return std::nullopt;
        return result;
    }

    /**
     * @brief Performs a checked multiplication operation on two integers.
     * @tparam T Integer type.
     * @param[in] a The left operand of the multiplication.
     * @param[in] b The right operand of the multiplication.
     * @return An std::optional containing the result if no overflow occurs, otherwise std::nullopt.
     */
    template<typename T>
        requires std::integral<T>
    std::optional<T> checked_mul(T a, T b) {
        T result;
        if (hardware_mul_overflow(a, b, &result)) return std::nullopt;
        return result;
    }

    /**
     * @brief Performs a checked division operation on two integers.
     * @tparam T Integer type.
     * @param[in] a The left operand of the division.
     * @param[in] b The right operand of the division.
     * @return
     * An std::optional containing the result,
     * if no undefined behavior (division by zero or overflow) occurs,
     * otherwise std::nullopt.
     */
    template<typename T>
        requires std::integral<T>
    std::optional<T> checked_div(T a, T b) {
        // Division by zero is undefined behavior.
        if (ub_div_zero(a, b)) return std::nullopt;
        // `INT_MIN / -1` overflow undefined behavior.
        if (ub_signed_int_min_div_minus_one(a, b)) return std::nullopt;
        return a / b;
    }

#pragma endregion

#pragma region Overflowing operations

    // YYC MARK:
    // overflowing_* function family return a tuple with 2 items as the result.
    // First is the wrapping value and second is a boolean indicates whether overflow occurs.

    /**
     * @brief The result returned by the overflow function family.
     * @details
     * The first item is the operation result.
     * The second item indicates whether an overflow has occurred. true means overflow, otherwise false.
     */
    template<typename T>
        requires std::integral<T>
    using OverflowingPair = std::pair<T, bool>;

    template<typename T>
        requires std::integral<T>
    OverflowingPair<T> overflowing_add(T a, T b) {
        T result;
        bool overflow = hardware_add_overflow(a, b, &result);
        return std::make_pair(result, overflow);
    }

    template<typename T>
        requires std::integral<T>
    OverflowingPair<T> overflowing_sub(T a, T b) {
        T result;
        bool overflow = hardware_sub_overflow(a, b, &result);
        return std::make_pair(result, overflow);
    }

    template<typename T>
        requires std::integral<T>
    OverflowingPair<T> overflowing_mul(T a, T b) {
        T result;
        bool overflow = hardware_mul_overflow(a, b, &result);
        return std::make_pair(result, overflow);
    }

    template<typename T>
        requires std::integral<T>
    OverflowingPair<T> overflowing_div(T a, T b) {
        // Division by zero is undefined behavior.
        if (ub_div_zero(a, b)) throw std::logic_error("div with 0");
        // `INT_MIN / -1` overflow undefined behavior.
        if (ub_signed_int_min_div_minus_one(a, b)) {
            // "a" self is minimum value, no need to get it again.
            return std::make_pair(a, true);
        } else {
            return std::make_pair(a / b, false);
        }
    }

#pragma endregion

#pragma region Saturating operations

    /*
    YYC MARK:
    The direction of saturation is pretty simple for unsigned integer.
    However, it is slightly complex for signed integer.
    In detail, it follow following rules:

    Acknowledge the truth for signed integer: abs(MIN) = abs(MAX) + 1
    - ADD operation:
        - Range operation rule: [a, b] + [c, d] = [a+c, b+d]
        - Pos+Pos -> [0, MAX] + [0, MAX] -> [0, 2 * MAX]. May overflow -> Saturating to MAX.
        - Neg+Neg -> [MIN, -1] + [MIN, -1] -> [2 * MIN, -2]. May underflow -> Saturating to MIN.
        - Pos+Neg -> [0, MAX] + [MIN, -1] -> [MIN, MAX - 1]. Impossible overflow or underflow.
    - SUB Operation:
        - Range operation rule: [a, b] - [c, d] = [a-d, b-c]
        - Pos-Neg -> [0, MAX] - [MIN, -1] -> [1, MAX - MIN]. Maybe overflow -> Saturating to MAX.
        - Neg-Pos -> [MIN, -1] - [0, MAX] -> [MIN - MAX, -1]. Maybe underflow -> Saturating to MIN.
        - Pos-Pos -> [0, MAX] - [0, MAX] -> [-MAX, MAX]. Impossible overflow or underflow.
        - Neg-Neg -> [MIN, -1] - [MIN, -1] -> [MIN + 1, -(MIN + 1)]. Impossible overflow or underflow.
    - MUL Operation:
        - Pos*Pos -> Maybe overflow -> Saturating to MAX.
        - Pos*Neg -> Maybe underflow -> Saturating to MIN.
        - Neg*Neg -> Maybe overflow -> Saturating to MAX.
    */

    /**
     * @brief Performs a saturating addition operation on two integers.
     * @details
     * If an overflow occurs during the addition, 
     * the result will be saturated to the maximum/minimum value,
     * according to the the direction of overflow (overflow or underflow).
     * @tparam T Integer type.
     * @param[in] a The left operand of the addition.
     * @param[in] b The right operand of the addition.
     * @return The result of the saturating addition operation.
     */
    template<typename T>
        requires std::integral<T>
    T saturating_add(T a, T b) {
        T result;
        if (hardware_add_overflow(a, b, &result)) {
            using Limits = std::numeric_limits<T>;
            if constexpr (std::is_unsigned_v<T>) {
                return Limits::max();
            } else {
                // Overflow only occurs when 2 operand have same sign.
                // So we simply check the sign of one of them.
                return (a > 0) ? Limits::max() : Limits::min();
            }
        }
        return result;
    }

    /**
     * @brief Performs a saturating subtraction operation on two integers.
     * @details
     * If an overflow occurs during the subtraction, 
     * the result will be saturated to the maximum/minimum value,
     * according to the the direction of overflow (overflow or underflow).
     * @tparam T Integer type.
     * @param[in] a The left operand of the subtraction.
     * @param[in] b The right operand of the subtraction.
     * @return The result of the saturating subtraction operation.
     */
    template<typename T>
        requires std::integral<T>
    T saturating_sub(T a, T b) {
        T result;
        if (hardware_sub_overflow(a, b, &result)) {
            using Limits = std::numeric_limits<T>;
            if constexpr (std::is_unsigned_v<T>) {
                return 0;
            } else {
                // Overflow only occurs when 2 operand have different sign.
                // So we simply compare these 2 operand.
                // If a < b, then "a" is negative, otherwise positive.
                return (a < b) ? Limits::min() : Limits::max();
            }
        }
        return result;
    }

    /**
     * @brief Performs a saturating multiplication operation on two integers.
     * @details
     * If an overflow occurs during the multiplication, 
     * the result will be saturated to the maximum/minimum value,
     * according to the the direction of overflow (overflow or underflow).
     * @tparam T Integer type.
     * @param[in] a The left operand of the multiplication.
     * @param[in] b The right operand of the multiplication.
     * @return The result of the saturating multiplication operation.
     */
    template<typename T>
        requires std::integral<T>
    T saturating_mul(T a, T b) {
        T result;
        if (hardware_mul_overflow(a, b, &result)) {
            using Limits = std::numeric_limits<T>;
            if constexpr (std::is_unsigned_v<T>) {
                return Limits::max();
            } else {
                // Check whether 2 operands have different sign.
                // If the result of XOR is true, these 2 operands should have different sign.
                return ((a ^ b) < 0) ? Limits::min() : Limits::max();
            }
        }
        return result;
    }

    /**
     * @brief Performs a saturating division operation on two integers.
     * @details
     * If an overflow occurs during the division, 
     * the result will be saturated to the maximum/minimum value,
     * according to the the direction of overflow (overflow or underflow).
     * @tparam T Integer type.
     * @param[in] a The left operand of the division.
     * @param[in] b The right operand of the division.
     * @return The result of the saturating division operation.
     * @exception std::logic_error If division by zero occurs.
     */
    template<typename T>
        requires std::integral<T>
    T saturating_div(T a, T b) {
        // Division by zero is undefined behavior.
        if (ub_div_zero(a, b)) throw std::logic_error("div with zero");
        // `INT_MIN / -1` overflow undefined behavior.
        if (ub_signed_int_min_div_minus_one(a, b)) {
            return std::numeric_limits<T>::max();
        } else {
            return a / b;
        }
    }

#pragma endregion

#pragma region Ordinary operations

    // YYC MARK:
    // Rust-way add, sub, mul and div operators.
    // There is no any undefined behavior which may occurs in these functions.
    // These normal operator is just an alias to wrapping_* function family.

    /**
     * @brief Performs an addition operation on two integers.
     * @tparam T Integer type.
     * @param[in] a The left operand of the addition.
     * @param[in] b The right operand of the addition.
     * @return The result of the addition operation.
     */
    template<typename T>
        requires std::integral<T>
    T add(T a, T b) {
        return wrapping_add(a, b);
    }

    /**
     * @brief Performs a subtraction operation on two integers.
     * @tparam T Integer type
     * @param[in] a The left operand of the subtraction.
     * @param[in] b The right operand of the subtraction.
     * @return The result of the subtraction operation.
     */
    template<typename T>
        requires std::integral<T>
    T sub(T a, T b) {
        return wrapping_sub(a, b);
    }

    /**
     * @brief Performs a multiplication operation on two integers.
     * @tparam T Integer type.
     * @param[in] a The left operand of the multiplication.
     * @param[in] b The right operand of the multiplication.
     * @return The result of the multiplication operation.
     */
    template<typename T>
        requires std::integral<T>
    T mul(T a, T b) {
        return wrapping_mul(a, b);
    }

    /**
     * @brief Performs a division operation on two integers.
     * @tparam T Integer type.
     * @param[in] a The left operand of the division.
     * @param[in] b The right operand of the division.
     * @return The result of the division operation.
     * @exception std::logic_error If division by zero or value overflow occurs.
     */
    template<typename T>
        requires std::integral<T>
    T div(T a, T b) {
        return wrapping_div(a, b);
    }

#pragma endregion

} // namespace yycc::num::safe_op

// YYC MARK:
// Delete the macro definition to prevent polluting the content later.
#undef YYCC_HARDWARE_OVERFLOW_GCC_FNS
#undef YYCC_HARDWARE_OVERFLOW_WIN32_FNS
