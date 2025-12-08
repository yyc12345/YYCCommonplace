#pragma once
#include "../../num/parse.hpp"
#include <optional>
#include <string_view>
#include <limits>
#include <concepts>
#include <cmath>

#define NS_YYCC_NUM_PARSE ::yycc::num::parse

namespace yycc::carton::clap::validator {

    /**
     * @brief The concept for checking whether given type is a valid command line argument validator.
     * @details
     * A valid command line argument validator must have a type name or alias called "ReturnType" 
     * indicating the return value of this validator.
     * 
     * And, it also should have an member function called "validate"
     * which receive <TT>const std::string_view&</TT> as its only argument,
     * and return <TT>std::optional<ReturnType></TT> as result.
     * If this \c std::optional is empty, it means that there is some error occurs when validating,
     * otherwise, it is the validated value.
     * 
     * Finally, it must can be default initialized.
     */
    template<typename T>
    concept Validator = std::default_initializable<T> && requires(const T& t, const std::u8string_view& sv) {
        // Check whether there is T::ReturnType type
        typename T::ReturnType;
        // Check whether there is "validate" member function and it has correct signature.
        { t.validate(sv) } -> std::same_as<std::optional<typename T::ReturnType>>;
    };

    /// @brief A convenient alias to the return type of validator.
    template<Validator T>
    using ValidatorReturnType = T::ReturnType;

    template<std::integral T, auto TMin = std::numeric_limits<T>::min(), auto TMax = std::numeric_limits<T>::max()>
    struct IntegralValidator {
        static_assert(TMin <= TMax);
        using ReturnType = T;
        std::optional<ReturnType> validate(const std::u8string_view& sv) {
            auto rv = NS_YYCC_NUM_PARSE::parse<ReturnType>(sv);
            if (rv) {
                auto value = rv.value();
                if (value > TMax || value < TMin) return std::nullopt;
                else return value;
            } else return std::nullopt;
        }
    };

    template<std::floating_point T, auto TMin = std::numeric_limits<T>::lowest(), auto TMax = std::numeric_limits<T>::max()>
    struct FloatingPointValidator {
        static_assert(std::isfinite(TMin));
        static_assert(std::isfinite(TMax));
        static_assert(TMin <= TMax);
        using ReturnType = T;
        std::optional<ReturnType> validate(const std::u8string_view& sv) {
            auto rv = NS_YYCC_NUM_PARSE::parse<ReturnType>(sv);
            if (rv) {
                auto value = rv.value();
                if (value > TMax || value < TMin) return std::nullopt;
                else return value;
            } else return std::nullopt;
        }
    };

    struct BoolValidator {
        using ReturnType = bool;
        std::optional<ReturnType> validate(const std::u8string_view& sv) {
            auto rv = NS_YYCC_NUM_PARSE::parse<ReturnType>(sv);
            if (rv) return rv.value();
            else return std::nullopt;
        }
    };

    struct StringValidator {
        using ReturnType = std::u8string;
        std::optional<ReturnType> validate(const std::u8string_view& sv) { return std::u8string(sv); }
    };

} // namespace yycc::carton::clap::validator

#undef NS_YYCC_NUM_PARSE
