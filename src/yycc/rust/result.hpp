#pragma once
#include "../macro/feature_probe.hpp"

#if defined(YYCC_CPPFEAT_EXPECTED)
#include <expected>
#endif

/// @brief The reproduction of Rust Option type.
/// @details
/// After writing a program in Rust, I deeply realized the advantages of Rust and its indispensable infrastructure, \c Result.
/// Therefore, it is essential to introduce Result into C++ to strengthen the security of the code.
/// I simulated Result in C++ as best I could, and its members Ok and Err (which are actually done by DeepSeek).
///
/// Why not write it the C++ way? Because the way C++ uses Result is too ugly and not explicit enough.
/// In the way C++ is written, the expected value is gotten by returning directly,
/// and if you encounter void specialization, you must write a pair of curly braces, which is very unclear.
/// For unexpected value, you need to manually build it by calling \c std::unexpected, which is more of a pain.
/// If you need to construct an unexpected value in place, you even need to put a \c std::in_place as the first argument to the constructor,
/// Otherwise, the std::unexpected constructor will not forward the given arguments to the underlying constructor.
///
///
/// @remarks
/// This namespace only work with environment supporting `std::expected` (i.e. C++ 23).
namespace yycc::rust::result {

#if defined(YYCC_CPPFEAT_EXPECTED)

    /**
     * @brief Equivalent Rust \c Result in C++
     */
    template<typename T, typename E>
    using Result = std::expected<T, E>;

    /**
     * @brief Equvialent Rust \c Result::Ok in C++
     * @param[in] args The arguments for building expected value.
     * @return An built Result instance with expected value.
     */
    template<typename ResultType, typename... Args>
    ResultType Ok(Args &&...args) {
        using T = ResultType::value_type;
        if constexpr (!std::is_void_v<T>) {
            return ResultType(std::in_place, std::forward<Args>(args)...);
        } else {
            static_assert(sizeof...(Args) == 0, "Ok<void> cannot accept arguments");
            return ResultType(std::in_place);
        }
    }

    /**
     * @brief Equvialent Rust \c Result::Err in C++
     * @param[in] args The arguments for building unexpected value.
     * @return An built Result instance with unexpected value.
     */
    template<typename ResultType, typename... Args>
    ResultType Err(Args &&...args) {
        return ResultType(std::unexpect, std::forward<Args>(args)...);
    }

    // TODO: Move these comments into Doxygen comments of this namespace.

    // YYC MARK:
    // Result类型中，类型E可以是根据你需求的任意值。
    // 在Rust中，一个非期望值类型Ea可以被转换为另一个非期望值类型Eb。
    // 这一特性通过From trait来实现。这样你就可以在一个函数中安全地将一种非期望值包装成另一种非期望值。
    // 但在C++中，我们有C++的方式来做同样的事。
    // 假设对于每一个类型E，我们都分别定义一个struct来描述它们，
    // 那么我们只需要为struct添加一些额外的构造函数，就可以将它们从一个类型转换到另一个。
    // ---
    // 例如，类型Ea是一个名为IoError的struct。
    // 在这个struct中，有一个类型为IoErrorKind的成员，指示了该IO错误的类别。
    // 与此同时，它还有一个以自己类型为唯一参数的构造函数，用于构建（复制或移动）它自己。
    // 现在在一个函数中。我们希望将它转换为另一个名为SystemError的类型Eb。
    // 你需要做的仅仅是新建一个名为SystemError的struct，然后为它编写所有必要的构造函数和其它函数。
    // 然后，重点是，为它添加一个形参类型为`const IoError&`的构造函数。
    // 这样一来，我们就可以通过类似这样的调用：`Err<Result<T, E>>(result.error());`，简单地将类型Ea转换为类型Eb。

    // YYC MARK:
    // 在Rust中，如果你想获得非预期值的人类可读说明，你就必须要实现名为Display的trait。
    // 而你不需要在C++中这样做，你必须编写自己的转换函数，以适应各种输出需求。
    // 例如，在使用std::format时，你需要编写适合std::format的格式化适配器。
    // 又如，你在使用std::cerr的operator<<重载时，你也需要编写合适的适配器。

#endif

}
