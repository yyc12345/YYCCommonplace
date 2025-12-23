#pragma once
#include "types.hpp"
#include "../../macro/class_copy_move.hpp"
#include <optional>
#include <string>
#include <vector>
#include <map>

#define NS_YYCC_CLAP_TYPES ::yycc::carton::clap::types

namespace yycc::carton::clap::variable {

    /// @brief Represents a command line variable that can be captured from user input.
    class Variable {
    public:
        /**
         * @brief Constructs a new Variable object with the specified parameters.
         * @param[in] name The name of the variable
         * @param[in] description A description of the variable's purpose
         * @param[in] care_value Whether the variable expects a value or not
         */
        Variable(const std::u8string_view& name, const std::u8string_view& description, bool care_value);
        ~Variable();
        YYCC_DEFAULT_COPY_MOVE(Variable)

    public:
        /**
         * @brief Checks if the variable cares about having a value associated with it.
         * @return true if the variable expects a value, false otherwise
         */
        bool is_care_value() const;
        /**
         * @brief Gets the name of the variable.
         * @return The variable's name as a u8string_view
         */
        std::u8string_view get_name() const;
        /**
         * @brief Gets the description of the variable.
         * @return The variable's description as a u8string_view
         */
        std::u8string_view get_description() const;

    private:
        bool care_value;           ///< Flag indicating whether the variable expects a value
        std::u8string name;        ///< The name of the variable
        std::u8string description; ///< A description of the variable's purpose
    };

    /// @brief Represents a variable that has been registered with a token for identification.
    class RegisteredVariable {
    public:
        /**
         * @private
         * @brief Constructs a new RegisteredVariable object with the given token and variable.
         * @param[in] token The unique token associated with the variable
         * @param[in] variable The variable to register
         */
        RegisteredVariable(NS_YYCC_CLAP_TYPES::Token token, Variable&& variable);
        ~RegisteredVariable();
        YYCC_DEFAULT_COPY_MOVE(RegisteredVariable)

    public:
        /**
         * @brief Gets the token associated with this registered variable.
         * @return The token identifying this variable
         */
        NS_YYCC_CLAP_TYPES::Token get_token() const;
        /**
         * @brief Gets the variable associated with this registration.
         * @return A constant reference to the variable
         */
        const Variable& get_variable() const;

    private:
        NS_YYCC_CLAP_TYPES::Token token; ///< The unique token identifying this variable
        Variable variable;               ///< The registered variable
    };

    /// @brief A collection of variables with methods to manage them by token or name.
    class VariableCollection {
    public:
        /**
         * @brief Constructs a new, empty VariableCollection object.
         */
        VariableCollection();
        ~VariableCollection();
        YYCC_DEFAULT_COPY_MOVE(VariableCollection)

    public:
        /**
         * @brief Adds a new variable to the collection and returns a token for it.
         * @param[in] var The variable to add to the collection
         * @return A token that uniquely identifies the added variable
         */
        NS_YYCC_CLAP_TYPES::Token add_variable(Variable&& var);
        /**
         * @brief Finds the token associated with a variable by its name.
         * @param[in] name The name of the variable to find
         * @return An optional token if the variable exists, std::nullopt otherwise
         */
        std::optional<NS_YYCC_CLAP_TYPES::Token> find_name(const std::u8string_view& name) const;
        /**
         * @brief Checks if a variable with the given token exists in the collection.
         * @param[in] token The token to check for
         * @return true if a variable with the token exists, false otherwise
         */
        bool has_variable(NS_YYCC_CLAP_TYPES::Token token) const;
        /**
         * @brief Gets the variable associated with the given token.
         * @param[in] token The token identifying the variable
         * @return A constant reference to the variable
         */
        const Variable& get_variable(NS_YYCC_CLAP_TYPES::Token token) const;
        /**
         * @brief Gets all registered variables in the collection.
         * @return A constant reference to the vector of registered variables
         */
        const std::vector<RegisteredVariable>& all_variables() const;
        /**
         * @brief Gets the number of variables in the collection.
         * @return The count of variables in the collection
         */
        size_t length() const;
        /**
         * @brief Checks if the collection is empty.
         * @return true if the collection contains no variables, false otherwise
         */
        bool empty() const;

    private:
        std::map<std::u8string, NS_YYCC_CLAP_TYPES::Token> names; ///< Map from variable names to tokens
        std::vector<RegisteredVariable> variables;                ///< Vector of registered variables
    };

} // namespace yycc::carton::clap::variable

#undef NS_YYCC_CLAP_TYPES
