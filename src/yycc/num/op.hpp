#pragma once
#include <stdexcept>
#include <concepts>

/**
 * @brief The namespace providing function relative robust numeric operations.
 * @details
 * 在使用Rust编写一些程序后，深刻地认识到Rust中针对基本类型的运算符的丰富程度。
 * 提供了诸如整数向上取整除法等便利操作。
 * 因此我在这个命名空间里复刻Rust中的这些便利功能。
 *
 * 由于没有需求，目前暂未实现以下功能：
 * \li 仅支持无符号整数向上取整除法。
 * \li 显式指定运算溢出行为
 */
namespace yycc::num::op {

    /**
     * @brief 无符号整数的向上整除
     * @details
     * 执行两个无符号整数之间的整除，并将结果向上取整。
     * 该函数可能在很多加密函数中进行使用以分配合适空间。
     * @tparam T 除法操作建立在的无符号整数类型
     * @param[in] lhs 左操作数
     * @param[in] rhs 右操作数
     * @return 向上取整的整除结果
     */
    template<typename T>
        requires std::unsigned_integral<T>
    T div_ceil(T lhs, T rhs) {
        if (rhs == 0) throw std::logic_error("div with 0");
        return (lhs % rhs == 0) ? (lhs / rhs) : (lhs / rhs) + 1u;
    }

}
