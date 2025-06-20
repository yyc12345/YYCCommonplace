#pragma once
#include "../constraint.hpp"
#include "../string.hpp"
#include <set>

#define NS_YYCC_STRING ::yycc::string

/// @brief The namespace containing convenient function building common used Constraint instance.
namespace yycc::constraint::builder {

    /**
     * @brief Build Constraint for arithmetic values by minimum and maximum value range.
     * @tparam T An arithmetic or enum type (except bool) of underlying stored value.
     * @param[in] min_value The minimum value of range (inclusive).
     * @param[in] max_value The maximum value of range (inclusive).
     * @return The generated constraint instance which can be directly applied.
    */
    template<typename T,
             std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, bool>, int> = 0>
    Constraint<T> min_max_constraint(T min_value, T max_value) {
        if (min_value > max_value)
            throw std::invalid_argument("the max value must be equal or greater than min value");

        auto fn_check = [min_value, max_value](const T& val) -> bool {
            return (val <= max_value) && (val >= min_value);
        };
        auto fn_clamp = [min_value, max_value](const T& val) -> T {
            return std::clamp(val, min_value, max_value);
        };
        return Constraint<T>(std::move(fn_check), std::move(fn_clamp));
    }

    /**
     * @brief Get constraint for enum values by enumerating all possible values.
     * @tparam T An enum type (except bool) of underlying stored value.
     * @param[in] il An initializer list storing all possible values.
     * @return The generated constraint instance which can be directly applied.
    */
    template<typename T, std::enable_if_t<std::is_enum_v<T>, int> = 0>
    Constraint<T> enum_constraint(const std::initializer_list<T>& il) {
        std::set<T> data(il);

        auto fn_check = [data](const T& val) -> bool { return data.find(val) != data.end(); };
        return Constraint<T>(std::move(fn_check), nullptr);
    }

    /**
     * @brief Get constraint for string values by enumerating all possible values.
     * @param[in] il An initializer list storing all possible values.
     * @return The generated constraint instance which can be directly applied.
     * @remarks
     * Caller must make sure that the string view passed in initializer list is valid until this Constraint life time gone.
     * Becasue this generator will not copy your given string view into string.
    */
    inline Constraint<NS_YYCC_STRING::u8string> GetStringEnumerationConstraint(
        const std::initializer_list<NS_YYCC_STRING::u8string_view>& il) {
        std::set<NS_YYCC_STRING::u8string_view> data(il);

        auto fn_check = [data](const NS_YYCC_STRING::u8string& val) -> bool {
            return data.find(NS_YYCC_STRING::u8string_view(val)) != data.end();
        };
        return Constraint<NS_YYCC_STRING::u8string>(std::move(fn_check), nullptr);
    }

} // namespace yycc::constraint::builder

#undef NS_YYCC_STRING
