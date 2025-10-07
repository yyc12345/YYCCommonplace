#pragma once
#include <expected>

/**
 * @brief The reproduction of Rust Option type.
 * @details
 * After writing programs in Rust, I deeply recognized the advantages of Rust and its indispensable infrastructure Result.
 * Therefore, introducing Result into C++ to enhance coding safety is essential.
 * I've done my best to simulate Rust's \c Result and its members \c Ok and \c Err (actually, I had DeepSeek simulate them).
 *
 * Why not write it in C++ style? Because C++'s way of using \c Result is too ugly and not explicit enough.
 * In C++'s approach, the expected value is returned directly, 
 * and when encountering void specialization, you must write a pair of curly braces, which is very unclear.
 * For unexpected values, you need to manually construct \c std::unexpected, which is even more painful.
 * If you need in-place construction of unexpected values, you even need to put \c std::in_place as the first parameter of the constructor,
 * otherwise \c std::unexpected 's constructor won't forward the subsequent parameters to the unexpected value's constructor.
 *
 * In the \c Result type, type \c E can be any value according to your needs.
 * In Rust, an unexpected value type \c Ea can be converted to another unexpected value type \c Eb.
 * This feature is implemented through the \c From trait, allowing you to safely wrap one type of unexpected value into another in a function.
 * But in C++, we have C++ ways to do the same thing.
 * Assuming for each type \c E, we define a separate struct to describe them,
 * then we just need to add some extra constructors to the struct to convert them from one type to another.
 * 
 * For example, type \c Ea is a struct named \c IoError.
 * In this struct, there is a member of type \c IoErrorKind indicating the category of this IO error.
 * At the same time, it has a constructor with its own type as the only parameter, used to construct (copy or move) itself.
 * Now in a function, we want to convert it to another type \c Eb named \c SystemError .
 * All you need to do is create a new struct named \c SystemError, then write all necessary constructors and other functions for it.
 * Then, the key point is to add a constructor with parameter type <TT>const IoError&</TT>.
 * This way, we can simply convert type \c Ea to type \c Eb through calls like: <TT>Err<Result<T, E>>(result.error());</TT>.
 *
 * In Rust, if you want to get human-readable descriptions of unexpected values, you must implement the \c Display trait.
 * But you don't need to do this in C++, you must write your own conversion functions to adapt to various output requirements.
 * For example, when using \c std::format, you need to write suitable formatting adapters for \c std::format.
 * Similarly, when using \c std::cerr 's \c operator<< overload, you also need to write suitable adapters.
 * @remarks This namespace only work with environment supporting `std::expected` (i.e. C++ 23).
 */
namespace yycc::result {

    /**
     * @brief Equivalent Rust \c Result in C++
     * @tparam T The type of the expected value.
     * @tparam E The type of the unexpected value.
     */
    template<typename T, typename E>
    using Result = std::expected<T, E>;

    /**
     * @brief Equvialent Rust \c Result::Ok in C++.
     * @tparam ResultType The type of the Result instance.
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
     * @brief Equvialent Rust \c Result::Err in C++.
     * @tparam ResultType The type of the Result instance.
     * @param[in] args The arguments for building unexpected value.
     * @return An built Result instance with unexpected value.
     */
    template<typename ResultType, typename... Args>
    ResultType Err(Args &&...args) {
        return ResultType(std::unexpect, std::forward<Args>(args)...);
    }

} // namespace yycc::result
