#pragma once
#include "../../macro/class_copy_move.hpp"
#include "types.hpp"
#include "application.hpp"
#include "validator.hpp"
#include <vector>
#include <utility>
#include <map>
#include <optional>
#include <string>
#include <string_view>

#define NS_YYCC_CLAP_TYPES ::yycc::carton::clap::types
#define NS_YYCC_CLAP_APPLICATION ::yycc::carton::clap::application
#define NS_YYCC_CLAP_VALIDATOR ::yycc::carton::clap::validator

namespace yycc::carton::clap::resolver {

    /// @brief Resolves environment variables and provides access to resolved values.
    class Resolver {
    private:
        /**
         * @brief All captured environment variable.
         * @details Key is the token to already registered variable.
         * Value is the associated value for key token.
         * If it is no-value variable, the value will be \c std::nullopt.
         */
        std::map<NS_YYCC_CLAP_TYPES::Token, std::optional<std::u8string>> values;

    public:
        /**
         * @brief Creates a Resolver from user-provided environment variables.
         * @param[in] app The application to resolve variables for
         * @param[in] vars Vector of key-value pairs representing environment variables
         * @return A ClapResult containing either a Resolver object or an error
         */
        static NS_YYCC_CLAP_TYPES::ClapResult<Resolver> from_user(
            const NS_YYCC_CLAP_APPLICATION::Application& app, const std::vector<std::pair<std::u8string_view, std::u8string_view>>& vars);
        /**
         * @brief Creates a Resolver from system environment variables.
         * @param[in] app The application to resolve variables for
         * @return A ClapResult containing either a Resolver object or an error
         */
        static NS_YYCC_CLAP_TYPES::ClapResult<Resolver> from_system(const NS_YYCC_CLAP_APPLICATION::Application& app);

    private:
        Resolver(decltype(Resolver::values)&& values);

    public:
        ~Resolver();
        YYCC_DEFAULT_COPY_MOVE(Resolver)

    private:
        /**
         * @brief Gets the raw value of a variable as a string view.
         * @param[in] token The token identifying the variable
         * @return A ClapResult containing either the raw value as a string view or an error
         */
        NS_YYCC_CLAP_TYPES::ClapResult<std::u8string_view> get_raw_value_variable(NS_YYCC_CLAP_TYPES::Token token) const;

    public:
        /**
         * @brief Checks if a variable with the given token was provided in the environment.
         * @param[in] token The token identifying the variable
         * @remark
         * Do not use this function for checking whether some variables are specified.
         * Please use get_flag_variable or get_value_variable instead.
         * Because this function can not distinguish flag variable and value variable.
         * This function onlt suit for user defined check,
         * for example, checking the constraint between some variables.
         * @return true if the variable was provided, false otherwise
         */
        bool has_variable(NS_YYCC_CLAP_TYPES::Token token) const;
        /**
         * @brief Gets the boolean value of a flag variable.
         * @param[in] token The token identifying the variable
         * @return A ClapResult containing either the boolean value 
         * representing whether user specify it or an error
         * @exception std::logic_error Get value variable as flag variable.
         */
        NS_YYCC_CLAP_TYPES::ClapResult<bool> get_flag_variable(NS_YYCC_CLAP_TYPES::Token token) const;
        /**
         * @brief Get the capture status of value variable.
         * @tparam T The validator of this value.
         * @param[in] token The token identifying the variable.
         * @param[in] validator The validator instance for checking or leave blank for default.
         * @return A ClapResult containing either the converted user input value or an error
         * @exception std::logic_error Get flag variable as value variable.
         */
        template<NS_YYCC_CLAP_VALIDATOR::Validator T>
        NS_YYCC_CLAP_TYPES::ClapResult<NS_YYCC_CLAP_VALIDATOR::ValidatorReturnType<T>> get_value_variable(
            NS_YYCC_CLAP_TYPES::Token token, const T& validator = T()) const {
            auto raw_value = this->get_raw_value_variable(token);
            if (raw_value.has_value()) {
                auto value = validator.validate(raw_value.value());
                if (value.has_value()) return value.value();
                else return std::unexpected(NS_YYCC_CLAP_TYPES::ClapError::BadCast);
            } else {
                return std::unexpected(raw_value.error());
            }
        }
    };

} // namespace yycc::carton::clap::resolver

#undef NS_YYCC_CLAP_VALIDATOR
#undef NS_YYCC_CLAP_APPLICATION
#undef NS_YYCC_CLAP_TYPES
