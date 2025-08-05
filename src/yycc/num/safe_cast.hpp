#pragma once
#include <expected>
#include <concepts>
#include <limits>
#include <type_traits>

/**
 * @brief The namespace providing functions which safely cast numeric value from one type to another.
 * @details
 * 在编写Rust的时候，深刻地认识到，在范围不同的类型之间进行转换是一件非常重要且容易出错的事情。
 * 对于拓宽转换，我们可以安全地，不加考虑地直接进行转换。
 * 对于收窄转换，我们则需要引入Result机制，来判断是否出错。
 * 这些功能在Rust中对所有基础类型都有实现，且通过From trait来进行统一管理，非常完美。
 * 但在C++中，我们需要手动重现它们。
 *
 * 在这个命名空间中，我们将转换函数分为了两类，一种是to，适用于肯定能安全转换的类型。
 * 另一种是try_to，用于可能存在风险的转换。
 * 但直接套用CAN_SAFE_TO<T, U>来决定是否可以安全转换是不正确的。
 * Rust中，这些转换安全与否是通过trait的不同（From和TryFrom）来手动编写的。
 * 而在这里我们粗暴地用编译时数据类型大小的比较结果来决定是否安全转换。
 * 这就导致那些变长基本数据类型在不同编译平台上致使CAN_SAFE_TO<T, U>输出不同结果，
 * 从而影响我们的代码的可移植性。
 * 例如在uint64_t和size_t之间的变换，在64位平台上我们可以使用to函数直接转换，
 * 然而在32位平台上，就会出现编译报错，造成了代码的可移植性问题。
 * Rust中的解决方案是Rust规定了Usize的最小大小（32位），只有小于其最小大小的数据才能安全的转换（例如u16）。
 * 但在C++中我们不能那么做，也无从做起，因为我们不能知道每一个变长基本数据类型的最小大小。
 * 所以我们使用另一种方案来解决这个问题。
 *
 * 我们的解决方案是，为to函数强制应用CAN_SAFE_TO<T, U>规则，而不对try_to函数强制应用。
 * 在try_to函数内部，我们再使用CAN_SAFE_TO<T, U>，判断是否可以安全转换，如果可以，就直接转换，否则做一系列判定。
 * 这样以来，程序员就需要手动判定两个数据类型之间是否肯定可以安全转换，然后再使用to函数。
 * 但至少，编译器会在你误使用to函数的时候，及时地给你抛出错误，这时候只要改用try_to就好了。
 * 同时，如果全篇使用try_to，也不会因为做了无用检查而影响性能，因为无用检查已经通过if constexpr删除了。
 *
 * 由于没有需求，目前不支持以下转换：
 * \li 浮点数与浮点数之间的转换。
 * \li 浮点数与整数之间的转换。
 */
namespace yycc::num::safe_cast {

    /// @brief All possible error raised in this module.
    enum class CastError {
        Overflow,  ///< 转换时发生向上溢出错误。
        Underflow, ///< 转换时发生向下溢出错误。
    };

    /// @brief The result type in this module.
    template<typename T>
    using Result = std::expected<T, CastError>;

    /**
     * @private
     * @brief 检查一个整数类型是否可以安全地转换为另一个整数类型
     * @return
     */
    template<typename TDst, typename TSrc>
        requires std::integral<TDst> && std::integral<TSrc>
    constexpr bool can_safe_to() {
        // 检查 TSrc 和 TDst 是否有符号
        constexpr bool is_src_signed = std::is_signed_v<TSrc>;
        constexpr bool is_dst_signed = std::is_signed_v<TDst>;

        // 获取 TSrc 和 TDst 的范围
        constexpr TSrc src_min = std::numeric_limits<TSrc>::min();
        constexpr TSrc src_max = std::numeric_limits<TSrc>::max();
        constexpr TDst dst_min = std::numeric_limits<TDst>::min();
        constexpr TDst dst_max = std::numeric_limits<TDst>::max();

        if constexpr (is_src_signed) {
            if constexpr (is_dst_signed) {
                // 有符号向有符号转换，两端都需要检查。
                // 如果完全处于范围内，则肯定可以安全转换。
                return dst_min <= src_min && dst_max >= src_max;
            } else {
                // 有符号向无符号转换，总是不安全的。
                // 因为会存在负数情况
                return false;
            }
        } else {
            if constexpr (is_dst_signed) {
                // 无符号向有符号转换，则只检查上端。
                // 如果上端够小内，则肯定可以安全转换。
                return dst_max >= src_max;
            } else {
                // 无符号向无符号转换，则只检查上端，因为下端均为0。
                return dst_max >= src_max;
            }
        }
    }

    /**
     * @private
     * @brief can_safe_to()的变量版本
     * @details 为后文约束提供的便利变量
     */
    template<typename TDst, typename TSrc>
        requires std::integral<TDst> && std::integral<TSrc>
    inline constexpr bool CAN_SAFE_TO = can_safe_to<TDst, TSrc>();

    /**
     * @brief 将一个整数类型转换为另一个整数类型。
     * @details 类似于Rust中的From<> trait，只不过颠倒了一下顺序（从from变为to）。
     * @return 转换后的结果。
     */
    template<typename TDst, typename TSrc>
        requires std::integral<TDst> && std::integral<TSrc> && CAN_SAFE_TO<TDst, TSrc>
    TDst to(const TSrc& lhs) {
        return static_cast<TDst>(lhs);
    }

    /**
     * @brief 尝试将一个整数类型转换为另一个整数类型。
     * @details 类似于Rust中的TryFrom<> trait，只不过颠倒了一下顺序（从from变为to）。
     * @return 一个包含转换结果的Result类型。
     */
    template<typename TDst, typename TSrc>
        requires std::integral<TDst> && std::integral<TSrc>
    Result<TDst> try_to(const TSrc& lhs) {
        // 检查是否可以直接转换
        if constexpr (CAN_SAFE_TO<TDst, TSrc>) {
            return static_cast<TDst>(lhs);
        } else {
            // 检查 TSrc 和 TDst 是否有符号
            constexpr bool is_src_signed = std::is_signed_v<TSrc>;
            constexpr bool is_dst_signed = std::is_signed_v<TDst>;

            // 获取 TSrc 和 TDst 的范围
            constexpr TSrc src_min = std::numeric_limits<TSrc>::min();
            constexpr TSrc src_max = std::numeric_limits<TSrc>::max();
            constexpr TDst dst_min = std::numeric_limits<TDst>::min();
            constexpr TDst dst_max = std::numeric_limits<TDst>::max();

            // 检查是否可以安全转换
            if constexpr (is_src_signed == is_dst_signed) {
                // 如果两者都是有符号或无符号，直接比较范围
                if (lhs < dst_min) return std::unexpected(CastError::Underflow);
                if (lhs > dst_max) return std::unexpected(CastError::Overflow);
                return static_cast<TDst>(lhs);
            } else {
                // 两者符号不一致
                if constexpr (is_src_signed) {
                    // 如果 TSrc 是有符号，TDst 是无符号，需要确保 lhs 不小于 0
                    if (lhs < 0) return std::unexpected(CastError::Underflow);
                    if (lhs > dst_max) return std::unexpected(CastError::Overflow);
                    return static_cast<TDst>(lhs);
                } else {
                    // 如果 TSrc 是无符号，TDst 是有符号，需要确保 lhs 不大于 TDst 的最大值
                    if (lhs > dst_max) return std::unexpected(CastError::Overflow);
                    return static_cast<TDst>(lhs);
                }
            }
        }
    }

} // namespace yycc::num::safe_cast
