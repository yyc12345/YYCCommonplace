#pragma once
#include "types.hpp"
#include "../../macro/class_copy_move.hpp"
#include <optional>
#include <string>
#include <vector>
#include <map>

#define NS_YYCC_CLAP_TYPES ::yycc::carton::clap::types

namespace yycc::carton::clap::variable {

    class Variable {
    public:
        Variable(const std::u8string_view& name, const std::u8string_view& description);
        ~Variable();
        YYCC_DEFAULT_COPY_MOVE(Variable)

    public:
        std::u8string_view get_name() const;
        std::u8string_view get_description() const;

    private:
        std::u8string name;
        std::u8string description;
    };

    class RegisteredVariable {
    public:
        RegisteredVariable(NS_YYCC_CLAP_TYPES::Token token, Variable&& variable);
        ~RegisteredVariable();
        YYCC_DEFAULT_COPY_MOVE(RegisteredVariable)

    public:
        NS_YYCC_CLAP_TYPES::Token get_token() const;
        const Variable& get_variable() const;

    private:
        NS_YYCC_CLAP_TYPES::Token token;
        Variable variable;
    };

    class VariableCollection {
    public:
        VariableCollection();
        ~VariableCollection();
        YYCC_DEFAULT_COPY_MOVE(VariableCollection)

    public:
        NS_YYCC_CLAP_TYPES::Token add_variable(Variable&& var);
        std::optional<NS_YYCC_CLAP_TYPES::Token> find_name(const std::u8string_view& name) const;
        bool has_variable(NS_YYCC_CLAP_TYPES::Token token) const;
        const Variable& get_variable(NS_YYCC_CLAP_TYPES::Token token) const;
        const std::vector<RegisteredVariable>& all_variables() const;
        size_t length() const;
        bool empty() const;

    private:
        std::map<std::u8string, NS_YYCC_CLAP_TYPES::Token> names;
        std::vector<RegisteredVariable> variables;
    };

} // namespace yycc::carton::clap::variable

#undef NS_YYCC_CLAP_TYPES
