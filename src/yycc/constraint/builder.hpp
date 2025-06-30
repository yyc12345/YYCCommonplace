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
     * @param[in] default_index The index of default value in given list.
     * @return The generated constraint instance which can be directly applied.
    */
    template<typename T, std::enable_if_t<std::is_enum_v<T>, int> = 0>
    Constraint<T> enum_constraint(const std::initializer_list<T>& il, size_t default_index = 0u) {
        if (default_index >= il.size())
            throw std::invalid_argument("the default index must be a valid index in given list");

        T default_entry = il.begin()[default_index];
        std::set<T> entries(il);

        // TODO: modify it as `contain` once we finish patch namespace.
        auto fn_check = [entries](const T& val) -> bool {
            return entries.find(val) != entries.end();
        };
        auto fn_clamp = [entries, default_entry](const T& val) -> T {
            if (entries.find(val) != entries.end()) {
                return val;
            } else {
                return default_entry;
            }
        };
        return Constraint<T>(std::move(fn_check), fn_clamp);
    }

    /**
     * @brief Get constraint for string values by enumerating all possible values.
     * @param[in] il An initializer list storing all possible values.
     * @param[in] default_index The index of default value in given list.
     * @return The generated constraint instance which can be directly applied.
    */
    inline Constraint<NS_YYCC_STRING::u8string> strenum_constraint(
        const std::initializer_list<NS_YYCC_STRING::u8string_view>& il, size_t default_index = 0u) {
        if (default_index >= il.size())
            throw std::invalid_argument("the default index must be a valid index in given list");

        NS_YYCC_STRING::u8string default_entry = NS_YYCC_STRING::u8string(il.begin()[default_index]);
        std::set<NS_YYCC_STRING::u8string> entries;
        for (const auto& i : il) {
            entries.emplace(i);
        }

        // TODO: modify it as `contain` once we finish patch namespace.
        auto fn_check = [entries](const NS_YYCC_STRING::u8string& val) -> bool {
            return entries.find(val) != entries.end();
        };
        auto fn_clamp = [entries, default_entry](
                            const NS_YYCC_STRING::u8string& val) -> NS_YYCC_STRING::u8string {
            if (entries.find(val) != entries.end()) {
                return val;
            } else {
                return default_entry;
            }
        };
        return Constraint<NS_YYCC_STRING::u8string>(std::move(fn_check), fn_clamp);
    }

} // namespace yycc::constraint::builder

#undef NS_YYCC_STRING
