#pragma once
#include "types.hpp"
#include "../../macro/class_copy_move.hpp"
#include <optional>
#include <string>
#include <vector>
#include <map>

#define NS_YYCC_CLAP_TYPES ::yycc::carton::clap::types

namespace yycc::carton::clap::option {

    class Option {
    public:
        Option(std::optional<std::u8string_view> short_name,
               std::optional<std::u8string_view> long_name,
               std::optional<std::u8string_view> value_hint,
               const std::u8string& description);
        ~Option();
        YYCC_DEFAULT_COPY_MOVE(Option)

    public:
        bool has_value() const;
        std::optional<std::u8string_view> get_short_name() const;
        std::optional<std::u8string_view> get_long_name() const;
        std::optional<std::u8string_view> get_value_hint() const;
        std::u8string_view get_description() const;

        std::u8string to_showcase_name() const;
        std::u8string to_showcase_value() const;

    private:
        static bool legal_short_name(const std::u8string_view& name);
        static bool legal_long_name(const std::u8string_view& name);

    private:
        std::optional<std::u8string> short_name;
        std::optional<std::u8string> long_name;
        std::optional<std::u8string> value_hint;
        std::u8string description;
    };

    class RegisteredOption {
    public:
        RegisteredOption(NS_YYCC_CLAP_TYPES::Token token, Option&& option);
        ~RegisteredOption();
        YYCC_DEFAULT_COPY_MOVE(RegisteredOption)

    public:
        NS_YYCC_CLAP_TYPES::Token get_token() const;
        const Option& get_option() const;

    private:
        NS_YYCC_CLAP_TYPES::Token token;
        Option option;
    };

    class OptionCollection {
    public:
        OptionCollection();
        ~OptionCollection();
        YYCC_DEFAULT_COPY_MOVE(OptionCollection)

    public:
        NS_YYCC_CLAP_TYPES::Token add_option(Option&& opt);
        std::optional<NS_YYCC_CLAP_TYPES::Token> find_long_name(const std::u8string_view& long_name) const;
        std::optional<NS_YYCC_CLAP_TYPES::Token> find_short_name(const std::u8string_view& short_name) const;
        bool has_option(NS_YYCC_CLAP_TYPES::Token token) const;
        const Option& get_option(NS_YYCC_CLAP_TYPES::Token token) const;
        const std::vector<RegisteredOption>& all_options() const;
        size_t length() const;
        bool empty() const;

    private:
        std::map<std::u8string, NS_YYCC_CLAP_TYPES::Token> short_names;
        std::map<std::u8string, NS_YYCC_CLAP_TYPES::Token> long_names;
        std::vector<RegisteredOption> options;
    };

} // namespace yycc::carton::clap::option

#undef NS_YYCC_CLAP_TYPES
