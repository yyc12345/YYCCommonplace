#pragma once
#include <type_traits>
#include <variant>

/**
 * @brief A stupid polyfill for std::expected,
 *
 * For those C++ standard which do not support std::expected,
 * we provide this namespace with a pretty bad but at least working std::expected pplyfill.
 *
 * The polyfill was done by std::variant.
 *
 * This namespace is used by this project because this framework must support C++ 17.
 */
namespace yycc::patch::expected {

    template<typename TValue, typename TError, std::enable_if_t<!std::is_same_v<TValue, TError>, int> = 0>
    using Expected = std::variant<TValue, TError>;

    template<typename TValue, typename TError>
    bool is_value(const Expected<TValue, TError>& exp) {
        return exp.index() == 0u;
    }

    template<typename TValue, typename TError>
    bool is_error(const Expected<TValue, TError>& exp) {
        return exp.index() == 1u;
    }

    template<typename TValue, typename TError>
    const TValue& get_value(const Expected<TValue, TError>& exp) {
        return std::get<0>(exp);
    }

    template<typename TValue, typename TError>
    const TError& get_error(const Expected<TValue, TError>& exp) {
        return std::get<1>(exp);
    }

}
