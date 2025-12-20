#pragma once
#include "../../macro/class_copy_move.hpp"
#include <vector>
#include <string>
#include <string_view>
#include <set>

namespace yycc::carton::brigadier::types {

    /**
     * @private
     * @brief A wrapper for argument visiting in brigadier.
     * @details
     * When using this stack, the top of stack is the first argument,
     * and the bottom of stack is the last argument for the convenient calling from brigadier.
     * 
     * Actually, in all usage, all poped items will be finally pushed with FILO order again.
     * So I simply use a vector and cursor to represent a fake stack.
     */
    class ArgumentStack {
    public:
        ArgumentStack(std::vector<std::u8string>&& args);
        ~ArgumentStack();
        YYCC_DEFAULT_COPY_MOVE(ArgumentStack)

    public:
        void reset();
        bool empty() const;
        std::u8string_view peek() const;
        void pop();
        void push();

    private:
        std::vector<std::u8string> stack;
        size_t cursor;
    };

    /// @private
    /// @brief Internal node used conflict set for detecting potential conflict.
    class ConflictSet {
    public:
        ConflictSet();
        ~ConflictSet();
        YYCC_DEFAULT_COPY_MOVE(ConflictSet)

    public:
        /**
		 * @brief Add literal item into conflict set.
		 * @param[in] value Literal item.
		 * @remarks
		 * \li Literal item is the string input in command line.
		 * \li The word in Literal, and the vocabulary in Choice should be put by this function.
		 * \li Added item will add \c literal: prefix to make it in literal scope,
		 * so that it will not be compared with argument name items.
		 * Because we allow 2 literal item and argument name item have same name.
		 */
        void add_literal(const std::u8string_view& value);
        /**
		 * @brief Add argument name item into conflict
		 * @param[in] value Argument name item.
		 * @remarks
		 * \li Argument name item is the key name put in ArgumentsMap.
		 * \li The argument name in Choice and Argument should be put by this function.
		 * \li Added item will add \c argument: prefix to make it in argument name scope,
		 * so that it will not be compared with literal items.
		 * Because we allow 2 literal item and argument name item have same name.
		 */
        void add_argument(const std::u8string_view& value);
        /**
		 * @brief Check whether this set is conflict with another.
		 * @param[in] rhs The set to be compared.
		 * @return True if they have conflict.
		 * @remarks
		 * This function simply compute the intersection of two set.
		 * If the result is not empty, it means that there is a conflict.
		 */
        bool is_conflict_with(const ConflictSet& rhs) const;

    private:
        std::set<std::u8string> inner;
    };

}

