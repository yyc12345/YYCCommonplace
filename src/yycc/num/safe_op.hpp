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
 */
namespace yycc::num::safe_op {

    // YYC MARK:
    // 在使用Rust编写一些程序后，深刻地认识到Rust中针对基本类型的运算符的丰富程度。
    // 可以显式地指定运算溢出的行为（wrapping，checked，overflowing，saturating四选一）
    // 因此我在这个命名空间里复刻Rust中的这些便利功能。

    // YYC MARK:
    // 实现说明：
    //     * Wrapping 运算：
    //         - 无符号整数直接使用默认溢出行为。
    //         - 有符号整数通过无符号类型转换模拟溢出回绕。
    //     * Checked 运算：
    //         - 使用编译器内置函数检测溢出，返回std::optional<T>。
    //         - 除零或溢出时返回std::nullopt。
    //     * Overflowing 运算：
    //         - 返回包含结果和溢出标志的结构体OverflowResult<T>。
    //         - 显式处理除零和有符号最小值除以-1的情况。
    //     * Saturating 运算：
    //         - 溢出时返回类型最大值或最小值。
    //         - 根据操作数符号判断饱和方向。

#pragma region Undefined Behaviors

    // YYC MARK:
    // 需要注意以下未定义行为：
    // * 有符号整数运算结果超出该类型表示范围时（如 INT_MAX + 1），行为未定义。
    // * 对 INT_MIN / -1 的除法可能溢出，导致UB。
    // * 无符号整数溢出时行为是明确定义的（按模 2^N 回绕），但需注意逻辑错误。
    // * 除数为零时行为未定义。

    /**
     * @private
     * @brief 两数相加，同时考虑有符号整数溢出的未定义行为
     * @param a 加法的左操作数
     * @param b 加法的右操作数
     * @return 考虑了有符号整数未定义行为的加法结果。
     */
    template<typename T>
        requires std::integral<T>
    T ub_signed_int_add(T a, T b) {
        if constexpr (std::is_unsigned_v<T>) {
            // 无符号数的加减乘都是自然回绕的，可以直接使用运算符。
            return a + b;
        } else {
            // 有符号数的溢出在C++中是未定义的。
            // 所以需要使用位操作强制转换为无符号进行计算，然后再转回来。
            using UT = std::make_unsigned_t<T>;
            return static_cast<T>(static_cast<UT>(a) + static_cast<UT>(b));
        }
    }

    /**
     * @private
     * @brief 两数相减，同时考虑有符号整数溢出的未定义行为
     * @param a 减法的左操作数
     * @param b 减法的右操作数
     * @return 考虑了有符号整数未定义行为的减法结果。
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
     * @brief 两数相乘，同时考虑有符号整数溢出的未定义行为
     * @param a 乘法的左操作数
     * @param b 乘法的右操作数
     * @return 考虑了有符号整数未定义行为的乘法结果。
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
     * @brief 检查有符号最小值除以-1时的未定义行为
     * @param a 除法的左操作数
     * @param b 除法的右操作数
     * @return 如果会发生未定义行为，则为true，否则为false
     */
    template<typename T>
        requires std::integral<T>
    bool ub_signed_int_min_div_minus_one(T a, T b) {
        if constexpr (std::is_signed_v<T>) {
            // 对有符号数来说， INT_MIN / -1 的除法可能溢出
            // （如 INT_MIN == -INT_MIN - 1 时），导致未定义行为。
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
     * @brief 检查除以0的未定义行为
     * @details 如果发生除以0，则引发panic
     * @param a 除法的左操作数
     * @param b 除法的右操作数
     * @return 如果会发生未定义行为，则为true，否则为false
     */
    template<typename T>
        requires std::integral<T>
    bool ub_div_zero([[maybe_unused]] T a, T b) {
        return b == 0;
    }

#pragma endregion

#pragma region Hardware Operation Overflow

// YYC MARK:
// 定义一个方便在Windows函数中计算的宏
#if defined(YYCC_HARDWARE_OVERFLOW_WIN32_FNS)
#define WIN_EASY_OPER(fn, ty, a, b, c) FAILED(fn(static_cast<ty>(a), static_cast<ty>(b), reinterpret_cast<ty*>(c)))
#endif

    /**
     * @private
     * @brief 基于硬件指令的带溢出检测的加法
     * @param[in] a 加法的左操作数
     * @param[in] b 加法的右操作数
     * @param[out] c 存放结果的引用
     * @return 如果发生溢出，则为true，否则为false。
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
        // 由于Windows函数限制，如果发生溢出，函数不会计算结果。
        // 所以我们要手动填写回绕了的结果
        if (overflow) *c = ub_signed_int_add<T>(a, b);
        return overflow;
#endif
    }

    /**
     * @private
     * @brief 基于硬件指令的带溢出检测的减法
     * @param[in] a 减法的左操作数
     * @param[in] b 减法的右操作数
     * @param[out] c 存放结果的引用
     * @return 如果发生溢出，则为true，否则为false。
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
        // 同理，手算溢出值
        if (overflow) *c = ub_signed_int_sub<T>(a, b);
        return overflow;
#endif
    }

    /**
     * @private
     * @brief 基于硬件指令的带溢出检测的乘法
     * @param[in] a 乘法的左操作数
     * @param[in] b 乘法的右操作数
     * @param[out] c 存放结果的引用
     * @return 如果发生溢出，则为true，否则为false。
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
        // 同理，手算溢出值
        if (overflow) *c = ub_signed_int_mul<T>(a, b);
        return overflow;
#endif
    }

// YYC MARK: 删除定义的宏，防止污染
#if defined(YYCC_HARDWARE_OVERFLOW_WIN32_FNS)
#undef WIN_EASY_OPER
#endif

#pragma endregion

#pragma region Wrapping operations

    // YYC MARK:
    // 使用 wrapping_* 方法在所有模式下进行包裹。

    template<typename T>
        requires std::integral<T>
    T wrapping_add(T a, T b) {
        return ub_signed_int_add(a, b);
    }

    template<typename T>
        requires std::integral<T>
    T wrapping_sub(T a, T b) {
        return ub_signed_int_sub(a, b);
    }

    template<typename T>
        requires std::integral<T>
    T wrapping_mul(T a, T b) {
        return ub_signed_int_mul(a, b);
    }

    template<typename T>
        requires std::integral<T>
    T wrapping_div(T a, T b) {
        // 除以0是未定义行为
        if (ub_div_zero(a, b)) throw std::logic_error("div with 0");
        // 对有符号数来说， INT_MIN / -1 的除法可能溢出
        // （如 INT_MIN == -INT_MIN - 1 时），导致未定义行为。
        if (ub_signed_int_min_div_minus_one(a, b))
            // 此时a就是有符号整数最小值，直接返回他即可。
            // 不需要再通过std::numeric_limits去访问最小值。
            return a;
        return a / b;
    }

#pragma endregion

#pragma region Checked operations

    // YYC MARK: 使用 checked_* 方法时发生溢出，则返回 None 值

    template<typename T>
        requires std::integral<T>
    std::optional<T> checked_add(T a, T b) {
        T result;
        if (hardware_add_overflow(a, b, &result)) return std::nullopt;
        return result;
    }

    template<typename T>
        requires std::integral<T>
    std::optional<T> checked_sub(T a, T b) {
        T result;
        if (hardware_sub_overflow(a, b, &result)) return std::nullopt;
        return result;
    }

    template<typename T>
        requires std::integral<T>
    std::optional<T> checked_mul(T a, T b) {
        T result;
        if (hardware_mul_overflow(a, b, &result)) return std::nullopt;
        return result;
    }

    template<typename T>
        requires std::integral<T>
    std::optional<T> checked_div(T a, T b) {
        // 除以0返回空值
        if (ub_div_zero(a, b)) return std::nullopt;
        // 溢出返回空值
        if (ub_signed_int_min_div_minus_one(a, b)) return std::nullopt;
        return a / b;
    }

#pragma endregion

#pragma region Overflowing operations

    // YYC MARK: 使用 overflowing_* 方法返回该值和一个指示是否存在溢出的布尔值

    /**
     * @brief Overflow系列运算函数返回的结果
     * @details
     * 第一项为运算结果。
     * 第二项为指示是否发生了溢出，true为溢出，否则为false。
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
        // 除以0需要panic
        if (ub_div_zero(a, b)) throw std::logic_error("div with 0");
        // 溢出只可能发生在有符号最小值除以-1上
        if (ub_signed_int_min_div_minus_one(a, b)) {
            // a就是最小值，无需再次获取
            return std::make_pair(a, true);
        } else {
            return std::make_pair(a / b, false);
        }
    }

#pragma endregion

#pragma region Saturating operations

    // YYC MARK: 使用 saturating_* 方法使值达到最小值或最大值
    // 无符号的溢出判定较为简单，有符号的则稍显复杂，具体遵守如下规则
    // 认识到对于有符号：abs(MIN) = abs(MAX) + 1
    // * 加法：
    //     - 区间运算[a, b] + [c, d] = [a+c, b+d]
    //     - 正+正 -> [0, MAX] + [0, MAX] -> [0, 2 * MAX]，可能上溢 -> 饱和到max
    //     - 负+负 -> [MIN, -1] + [MIN, -1] -> [2 * MIN, -2]，可能下溢 -> 饱和到min
    //     - 正+负 -> [0, MAX] + [MIN, -1] -> [MIN, MAX - 1]，不可能溢出
    // * 减法：
    //     - 区间运算[a, b] - [c, d] = [a-d, b-c]
    //     - 正-负 -> [0, MAX] - [MIN, -1] -> [1, MAX - MIN]，可能上溢 -> 饱和到max
    //     - 负-正 -> [MIN, -1] - [0, MAX] -> [MIN - MAX, -1]，可能下溢 -> 饱和到min
    //     - 正-正 -> [0, MAX] - [0, MAX] -> [-MAX, MAX]，不可能溢出
    //     - 负-负 -> [MIN, -1] - [MIN, -1] -> [MIN + 1, -(MIN + 1)]，不可能溢出
    // * 乘法：
    //     - 正*正 -> 可能上溢 -> 饱和到max
    //     - 正*负 -> 可能下溢 -> 饱和到min
    //     - 负*负 -> 可能上溢 -> 饱和到max

    template<typename T>
        requires std::integral<T>
    T saturating_add(T a, T b) {
        T result;
        if (hardware_add_overflow(a, b, &result)) {
            using Limits = std::numeric_limits<T>;
            if constexpr (std::is_unsigned_v<T>) {
                return Limits::max();
            } else {
                // 溢出只可能发生在同号情况，因此只判定其中一个操作数的符号即可。
                return (a > 0) ? Limits::max() : Limits::min();
            }
        }
        return result;
    }

    template<typename T>
        requires std::integral<T>
    T saturating_sub(T a, T b) {
        T result;
        if (hardware_sub_overflow(a, b, &result)) {
            using Limits = std::numeric_limits<T>;
            if constexpr (std::is_unsigned_v<T>) {
                return 0;
            } else {
                // 溢出只可能发生在异号情况，因此只判定两个操作数的大小即可。
                // a < b，则a为负，否则a为正
                return (a < b) ? Limits::min() : Limits::max();
            }
        }
        return result;
    }

    template<typename T>
        requires std::integral<T>
    T saturating_mul(T a, T b) {
        T result;
        if (hardware_mul_overflow(a, b, &result)) {
            using Limits = std::numeric_limits<T>;
            if constexpr (std::is_unsigned_v<T>) {
                return Limits::max();
            } else {
                // 做异号判定，如果XOR为true，则为异号
                return ((a ^ b) < 0) ? Limits::min() : Limits::max();
            }
        }
        return result;
    }

    template<typename T>
        requires std::integral<T>
    T saturating_div(T a, T b) {
        // 除以0需要panic
        if (ub_div_zero(a, b)) throw std::logic_error("div with zero");
        // 如果发生最小值除以0的情况，那么溢出到最大值
        if (ub_signed_int_min_div_minus_one(a, b)) {
            return std::numeric_limits<T>::max();
        } else {
            return a / b;
        }
    }

#pragma endregion

#pragma region Ordinary operations

    // YYC MARK:
    // 以Rust的方式进行四则运算，不存在未定义行为。
    // 默认的四则运算与wrapping_*系列函数行为一致。

    template<typename T>
        requires std::integral<T>
    T add(T a, T b) {
        return wrapping_add(a, b);
    }

    template<typename T>
        requires std::integral<T>
    T sub(T a, T b) {
        return wrapping_sub(a, b);
    }

    template<typename T>
        requires std::integral<T>
    T mul(T a, T b) {
        return wrapping_mul(a, b);
    }

    template<typename T>
        requires std::integral<T>
    T div(T a, T b) {
        return wrapping_div(a, b);
    }

#pragma endregion

} // namespace yycc::num::safe_op

// YYC MARK: 删除宏定义，防止污染后面的内容
#undef YYCC_HARDWARE_OVERFLOW_GCC_FNS
#undef YYCC_HARDWARE_OVERFLOW_WIN32_FNS
