#pragma once
#include "types.hpp"
#include "../../macro/class_copy_move.hpp"
#include <optional>
#include <string>
#include <vector>
#include <map>

#define NS_YYCC_CLAP_TYPES ::yycc::carton::clap::types

namespace yycc::carton::clap::option {

    /// @brief Represents a command line option that can be parsed from user input.
    class Option {
    public:
        /**
         * @brief Constructs a new Option object with the specified parameters.
         * @param[in] short_name Optional short name for the option (e.g., "h" for "-h")
         * @param[in] long_name Optional long name for the option (e.g., "help" for "--help")
         * @param[in] value_hint Optional hint for the expected value type (e.g., "FILE")
         * @param[in] description A description of the option's purpose
         */
        Option(std::optional<std::u8string_view> short_name,
               std::optional<std::u8string_view> long_name,
               std::optional<std::u8string_view> value_hint,
               const std::u8string_view& description);
        ~Option();
        YYCC_DEFAULT_COPY_MOVE(Option)

    public:
        /**
         * @brief Checks if the option expects a value.
         * @return true if the option expects a value, false otherwise
         */
        bool has_value() const;
        /**
         * @brief Gets the short name of the option.
         * @return An optional containing the short name if it exists, std::nullopt otherwise
         */
        std::optional<std::u8string_view> get_short_name() const;
        /**
         * @brief Gets the long name of the option.
         * @return An optional containing the long name if it exists, std::nullopt otherwise
         */
        std::optional<std::u8string_view> get_long_name() const;
        /**
         * @brief Gets the value hint for the option.
         * @return An optional containing the value hint if it exists, std::nullopt otherwise
         */
        std::optional<std::u8string_view> get_value_hint() const;
        /**
         * @brief Gets the description of the option.
         * @return The option's description as a u8string_view
         */
        std::u8string_view get_description() const;

        std::u8string to_showcase_name() const;
        std::u8string to_showcase_value() const;

    private:
        static bool legal_short_name(const std::u8string_view& name);
        static bool legal_long_name(const std::u8string_view& name);

    private:
        std::optional<std::u8string> short_name; ///< The short name of the option (if any)
        std::optional<std::u8string> long_name;  ///< The long name of the option (if any)
        std::optional<std::u8string> value_hint; ///< Hint for the expected value type (if any)
        std::u8string description;               ///< Description of the option's purpose
    };

    /// @brief Represents an option that has been registered with a token for identification.
    class RegisteredOption {
    public:
        /**
         * @private
         * @brief Constructs a new RegisteredOption object with the given token and option.
         * @param[in] token The unique token associated with the option
         * @param[in] option The option to register
         */
        RegisteredOption(NS_YYCC_CLAP_TYPES::Token token, Option&& option);
        ~RegisteredOption();
        YYCC_DEFAULT_COPY_MOVE(RegisteredOption)

    public:
        /**
         * @brief Gets the token associated with this registered option.
         * @return The token identifying this option
         */
        NS_YYCC_CLAP_TYPES::Token get_token() const;
        /**
         * @brief Gets the option associated with this registration.
         * @return A constant reference to the option
         */
        const Option& get_option() const;

    private:
        NS_YYCC_CLAP_TYPES::Token token; ///< The unique token identifying this option
        Option option;                   ///< The registered option
    };

    /// @brief A collection of options with methods to manage them by token or name.
    class OptionCollection {
    public:
        /// @brief Constructs a new, empty OptionCollection object.
        OptionCollection();
        ~OptionCollection();
        YYCC_DEFAULT_COPY_MOVE(OptionCollection)

    public:
        /**
         * @brief Adds a new option to the collection and returns a token for it.
         * @param[in] opt The option to add to the collection
         * @return A token that uniquely identifies the added option
         */
        NS_YYCC_CLAP_TYPES::Token add_option(Option&& opt);
        /**
         * @brief Finds the token associated with an option by its long name.
         * @param[in] long_name The long name of the option to find
         * @return An optional token if the option exists, std::nullopt otherwise
         */
        std::optional<NS_YYCC_CLAP_TYPES::Token> find_long_name(const std::u8string_view& long_name) const;
        /**
         * @brief Finds the token associated with an option by its short name.
         * @param[in] short_name The short name of the option to find
         * @return An optional token if the option exists, std::nullopt otherwise
         */
        std::optional<NS_YYCC_CLAP_TYPES::Token> find_short_name(const std::u8string_view& short_name) const;
        /**
         * @brief Checks if an option with the given token exists in the collection.
         * @param[in] token The token to check for
         * @return true if an option with the token exists, false otherwise
         */
        bool has_option(NS_YYCC_CLAP_TYPES::Token token) const;
        /**
         * @brief Gets the option associated with the given token.
         * @param[in] token The token identifying the option
         * @return A constant reference to the option
         */
        const Option& get_option(NS_YYCC_CLAP_TYPES::Token token) const;
        /**
         * @brief Gets all registered options in the collection.
         * @return A constant reference to the vector of registered options
         */
        const std::vector<RegisteredOption>& all_options() const;
        /**
         * @brief Gets the number of options in the collection.
         * @return The count of options in the collection
         */
        size_t length() const;
        /**
         * @brief Checks if the collection is empty.
         * @return true if the collection contains no options, false otherwise
         */
        bool empty() const;

    private:
        std::map<std::u8string, NS_YYCC_CLAP_TYPES::Token> short_names; ///< Map from short option names to tokens
        std::map<std::u8string, NS_YYCC_CLAP_TYPES::Token> long_names;  ///< Map from long option names to tokens
        std::vector<RegisteredOption> options;                          ///< Vector of registered options
    };

} // namespace yycc::carton::clap::option

#undef NS_YYCC_CLAP_TYPES
