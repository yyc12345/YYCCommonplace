#include "variable.hpp"
#include "../../patch/format.hpp"
#include <stdexcept>
#include <format>

#define TYPES ::yycc::carton::clap::types

namespace yycc::carton::clap::variable {

#pragma region Variable

    Variable::Variable(const std::u8string_view &name, const std::u8string_view &description) : name(name), description(description) {
        if (name.empty()) {
            throw std::logic_error("the name of variable should not be empty");
        }
    }

    Variable::~Variable() {}

    std::u8string_view Variable::get_name() const {
        return this->name;
    }

    std::u8string_view Variable::get_description() const {
        return this->description;
    }

#pragma endregion

#pragma region Registered Variable

    RegisteredVariable::RegisteredVariable(TYPES::Token token, Variable &&variable) : token(token), variable(std::move(variable)) {}

    RegisteredVariable::~RegisteredVariable() {}

    TYPES::Token RegisteredVariable::get_token() const {
        return this->token;
    }

    const Variable &RegisteredVariable::get_variable() const {
        return this->variable;
    }

#pragma endregion

#pragma region Variable Collection

    VariableCollection::VariableCollection() : names(), variables() {}

    VariableCollection::~VariableCollection() {}

    TYPES::Token VariableCollection::add_variable(Variable &&var) {
        auto token = this->variables.size();

        std::u8string name(var.get_name());
        auto [_, ok] = this->names.try_emplace(name, token);
        if (!ok) {
            throw std::logic_error(std::format("duplicated variable name {}", name));
        }

        this->variables.emplace_back(RegisteredVariable(token, std::move(var)));
        return token;
    }

    std::optional<TYPES::Token> VariableCollection::find_name(const std::u8string_view &name) const {
        auto finder = this->names.find(std::u8string(name));
        if (finder == this->names.end()) return std::nullopt;
        else return finder->second;
    }

    bool VariableCollection::has_variable(TYPES::Token token) const {
        return token < this->variables.size();
    }

    const Variable &VariableCollection::get_variable(TYPES::Token token) const {
        return this->variables.at(token).get_variable();
    }

    const std::vector<RegisteredVariable> &VariableCollection::all_variables() const {
        return this->variables;
    }

    size_t VariableCollection::length() const {
        return this->variables.size();
    }

    bool VariableCollection::empty() const {
        return this->variables.empty();
    }

#pragma endregion

} // namespace yycc::carton::clap::variable
