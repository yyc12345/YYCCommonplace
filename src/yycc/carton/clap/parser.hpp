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

namespace yycc::carton::clap::parser {

    /// @brief Parses command line arguments and provides access to parsed values.
    class Parser {
    private:
        /**
         * @brief All captured commandline argument.
         * @details Key is the token to already registered option.
         * Value is the associated value for key token.
         * If it is no-value option, the value will be \c std::nullopt.
         */
        std::map<NS_YYCC_CLAP_TYPES::Token, std::optional<std::u8string>> values;

    public:
        /**
         * @brief Creates a Parser from user-provided command line arguments.
         * @param[in] app The application to parse arguments for
         * @param[in] args Vector of command line argument strings to parse
         * @return A ClapResult containing either a Parser object or an error
         */
        static NS_YYCC_CLAP_TYPES::ClapResult<Parser> from_user(const NS_YYCC_CLAP_APPLICATION::Application& app,
                                                                const std::vector<std::u8string_view>& args);
        /**
         * @brief Creates a Parser from system command line arguments (argc/argv).
         * @param[in] app The application to parse arguments for
         * @return A ClapResult containing either a Parser object or an error
         */
        static NS_YYCC_CLAP_TYPES::ClapResult<Parser> from_system(const NS_YYCC_CLAP_APPLICATION::Application& app);

    private:
        Parser(decltype(Parser::values)&& values);

    public:
        ~Parser();
        YYCC_DEFAULT_COPY_MOVE(Parser)

    private:
        /**
         * @brief Gets the raw value of an option as a string view.
         * @param[in] token The token identifying the option
         * @return A ClapResult containing either the raw value as a string view or an error
         */
        NS_YYCC_CLAP_TYPES::ClapResult<std::u8string_view> get_raw_value_option(NS_YYCC_CLAP_TYPES::Token token) const;

    public:
        /**
         * @brief Checks if an option with the given token was provided in the command line.
         * @param[in] token The token identifying the option
         * @remark
         * Do not use this function for checking whether some options are specified.
         * Please use get_flag_option or get_value_option instead.
         * Because this function can not distinguish flag option and value option.
         * This function onlt suit for user defined check,
         * for example, checking the constraint between some options.
         * @return true if the option was provided, false otherwise
         */
        bool has_option(NS_YYCC_CLAP_TYPES::Token token) const;
        /**
         * @brief Get the capture status of flag options.
         * @param[in] token The token identifying the option
         * @return A ClapResult containing either the boolean value 
         * representing whether user specify it or an error
         * @exception std::logic_error Get value option as flag option.
         */
        NS_YYCC_CLAP_TYPES::ClapResult<bool> get_flag_option(NS_YYCC_CLAP_TYPES::Token token) const;
        /**
         * @brief Get the capture status of value options.
         * @tparam T The validator of this value.
         * @param[in] token The token identifying the option.
         * @param[in] validator The validator instance for checking or leave blank for default.
         * @return A ClapResult containing either the converted user input value or an error
         * @exception std::logic_error Get flag option as value option.
         */
        template<NS_YYCC_CLAP_VALIDATOR::Validator T>
        NS_YYCC_CLAP_TYPES::ClapResult<NS_YYCC_CLAP_VALIDATOR::ValidatorReturnType<T>> get_value_option(NS_YYCC_CLAP_TYPES::Token token,
                                                                                                        const T& validator = T{}) const {
            auto raw_value = this->get_raw_value_option(token);
            if (raw_value.has_value()) {
                auto value = validator.validate(raw_value.value());
                if (value.has_value()) return value.value();
                else return std::unexpected(NS_YYCC_CLAP_TYPES::ClapError::BadCast);
            } else {
                return std::unexpected(raw_value.error());
            }
        }
    };

} // namespace yycc::carton::clap::parser

#undef NS_YYCC_CLAP_VALIDATOR
#undef NS_YYCC_CLAP_APPLICATION
#undef NS_YYCC_CLAP_TYPES
