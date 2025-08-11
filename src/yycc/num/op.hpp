#pragma once
#include <stdexcept>
#include <concepts>

/**
 * @brief The namespace providing functions for robust numeric operations.
 * @details
 * After writing some programs in Rust, I deeply appreciated the richness of operators
 * for primitive types in Rust, which provides convenient operations like ceiling integral division.
 * Therefore, I replicate these convenient features from Rust in this namespace.
 *
 * Currently unimplemented features due to lack of demand:
 * \li Only supports unsigned integer ceiling division
 */
namespace yycc::num::op {

    /**
     * @brief Unsigned integer ceiling division
     * @details
     * Performs division between two unsigned integers and rounds up the result.
     * @exception std::logic_error If the divisor is zero
     * @tparam T The unsigned integer type for division operation
     * @param[in] lhs Left operand
     * @param[in] rhs Right operand
     * @return Ceiling division result
     */
    template<typename T>
        requires std::unsigned_integral<T>
    T div_ceil(T lhs, T rhs) {
        // Check divisor first
        if (rhs == 0) throw std::logic_error("div with 0");
        // YYC MARK:
        // We use this algorithm, instead of traditional `(lhs + rhs - 1) / rhs`, 
        // which may have unsafe overflow case.
        return (lhs % rhs == 0) ? (lhs / rhs) : (lhs / rhs) + 1u;
    }

}
