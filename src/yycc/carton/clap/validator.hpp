#pragma once
#include "../../macro/class_copy_move.hpp"
#include "../../num/parse.hpp"
#include <optional>
#include <string_view>
#include <limits>
#include <concepts>
#include <stdexcept>
#include <cmath>

#define NS_YYCC_NUM_PARSE ::yycc::num::parse

namespace yycc::carton::clap::validator {

    /**
     * @brief The concept for checking whether given type is a valid command line argument validator.
     * @details
     * A valid command line argument validator must have a type name or alias called "ReturnType" 
     * indicating the return value of this validator.
     * Please note that the type of this return value must can be placed into \c std::optional.
     * 
     * And, it also should have an member function called "validate"
     * which receive <TT>const std::string_view&</TT> as its only argument,
     * and return <TT>std::optional<ReturnType></TT> as result.
     * If this \c std::optional is empty, it means that there is some error occurs when validating,
     * otherwise, it is the validated value.
     * 
     * Finally, it must can be default initialized.
     * Please note that this class may be initialized during each calling.
     * Please make sure that its ctor will not consume too much resources.
     */
    template<typename T>
    concept Validator = requires(const T& t, const std::u8string_view& sv) {
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
        IntegralValidator() = default;
        YYCC_DEFAULT_COPY_MOVE(IntegralValidator)

        static_assert(TMin <= TMax);

        using ReturnType = T;
        std::optional<ReturnType> validate(const std::u8string_view& sv) const {
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
        FloatingPointValidator() {
            // TODO: Remove this and make it "= default" when 3 common STL make std::isfinite become constexpr.
            if (!std::isfinite(TMin)) throw std::logic_error("invalid float minimum value.");
            if (!std::isfinite(TMax)) throw std::logic_error("invalid float maximum value.");
        }
        YYCC_DEFAULT_COPY_MOVE(FloatingPointValidator)

        // TODO: Use static_assert once 3 common STL make std::isfinite become constexpr.
        //static_assert(std::isfinite<T>(TMin));
        //static_assert(std::isfinite<T>(TMax));
        static_assert(TMin <= TMax);

        using ReturnType = T;
        std::optional<ReturnType> validate(const std::u8string_view& sv) const {
            auto rv = NS_YYCC_NUM_PARSE::parse<ReturnType>(sv);
            if (rv) {
                auto value = rv.value();
                if (value > TMax || value < TMin) return std::nullopt;
                else return value;
            } else return std::nullopt;
        }
    };

    struct BoolValidator {
        BoolValidator() = default;
        YYCC_DEFAULT_COPY_MOVE(BoolValidator)

        using ReturnType = bool;
        std::optional<ReturnType> validate(const std::u8string_view& sv) const {
            auto rv = NS_YYCC_NUM_PARSE::parse<ReturnType>(sv);
            if (rv) return rv.value();
            else return std::nullopt;
        }
    };

    struct StringValidator {
        StringValidator() = default;
        YYCC_DEFAULT_COPY_MOVE(StringValidator)

        using ReturnType = std::u8string;
        std::optional<ReturnType> validate(const std::u8string_view& sv) const { return std::u8string(sv); }
    };

} // namespace yycc::carton::clap::validator

#undef NS_YYCC_NUM_PARSE
