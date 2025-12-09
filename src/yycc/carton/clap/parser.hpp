#pragma once
#include "../../macro/class_copy_move.hpp"
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
        static NS_YYCC_CLAP_TYPES::ClapResult<Parser> from_user(const NS_YYCC_CLAP_APPLICATION::Application& app,
                                                                const std::vector<std::u8string_view>& args);
        static NS_YYCC_CLAP_TYPES::ClapResult<Parser> from_system(const NS_YYCC_CLAP_APPLICATION::Application& app);

    private:
        Parser(decltype(Parser::values)&& value);

    public:
        ~Parser();
        YYCC_DEFAULT_COPY_MOVE(Parser)

    private:
        NS_YYCC_CLAP_TYPES::ClapResult<std::u8string_view> get_raw_value_option(NS_YYCC_CLAP_TYPES::Token token) const;

    public:
        bool has_option(NS_YYCC_CLAP_TYPES::Token token) const;
        NS_YYCC_CLAP_TYPES::ClapResult<bool> get_flag_option(NS_YYCC_CLAP_TYPES::Token token) const;
        template<NS_YYCC_CLAP_VALIDATOR::Validator T>
        NS_YYCC_CLAP_TYPES::ClapResult<NS_YYCC_CLAP_VALIDATOR::ValidatorReturnType<T>> get_value_option(
            NS_YYCC_CLAP_TYPES::Token token) const {
            auto raw_value = this->get_raw_value_option(token);
            if (raw_value.has_value()) {
                T validator{};
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
