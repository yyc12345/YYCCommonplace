#include "types.hpp"
#include "../../patch/format.hpp"
#include <stdexcept>
#include <algorithm>

#define FORMAT ::yycc::patch::format

namespace yycc::carton::brigadier::types {

#pragma region ArgumentStack

    ArgumentStack::ArgumentStack(std::vector<std::u8string> &&args) : stack(std::move(args)), cursor(0) {}

    ArgumentStack::~ArgumentStack() {}

    void ArgumentStack::reset() {
        this->cursor = 0;
    }

    bool ArgumentStack::empty() const {
        return cursor >= this->stack.size();
    }

    std::u8string_view ArgumentStack::peek() const {
        return std::u8string_view(this->stack.at(this->cursor));
    }

    void ArgumentStack::push() {
        if (cursor == 0) throw std::logic_error("no arguments can be pushed");
        else --cursor;
    }

    void ArgumentStack::push() {
        if (cursor >= this->stack.size()) throw std::logic_error("no arguments can be poped");
        else ++cursor;
    }

#pragma endregion

#pragma region ConflictSet

    ConflictSet::ConflictSet() : inner() {}

    ConflictSet::~ConflictSet() {}

    void ConflictSet::add_literal(const std::u8string_view &value) {
        if (value.empty()) throw std::invalid_argument("try to insert empty item to conflict set.");
        auto entry = FORMAT::format(u8"literal:{}", value);

        auto result = this->inner.emplace(std::move(entry));
        if (!result.second) throw std::runtime_error("try to insert duplicated item in conflict set.");
    }

    void ConflictSet::add_argument(const std::u8string_view &value) {
        if (value.empty()) throw std::invalid_argument("try to insert empty item to conflict set.");
        auto entry = FORMAT::format(u8"argument:{}", value);

        auto result = this->inner.emplace(std::move(entry));
        if (!result.second) throw std::runtime_error("try to insert duplicated item in conflict set.");
    }

    bool ConflictSet::is_conflict_with(const ConflictSet &rhs) const {
        // create a cache to store computed intersection
        std::vector<std::u8string> intersection;
        // compute intersection
        std::set_intersection(this->inner.begin(), this->inner.end(), rhs.inner.begin(), rhs.inner.begin(), std::back_inserter(intersection));
        // check whether it is empty intersection
        return !intersection.empty();
    }

#pragma endregion

} // namespace yycc::carton::brigadier::types
