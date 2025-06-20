#pragma once
#include "macro/class_copy_move.hpp"
#include <functional>
#include <stdexcept>
#include <algorithm>

/// @brief The namespace containing generic constraint concept used varied in other modules.
namespace yycc::constraint {

    /// @brief Function prototype used in Constraint for checking whether given value is valid.
    /// @details Analyze given value, and return true if value is legal, otherwise false.
    template<typename T>
    using FnCheck = std::function<bool(const T&)>;
    /// @brief Function prototype used in Constraint for clamping given value into a valid value.
    /// @details Analyze given value, return clamped value.
    template<typename T>
    using FnClamp = std::function<T(const T&)>;

    /**
	 * @brief The constraint applied to settings to limit its stored value.
	 * @tparam T The data type this constraint need to be processed with.
     * @details
     * Constraint class contains various features:
     * \li Check: Check whether given value is in range.
     * \li Clamp: Clamp given value into valid value.
     * Every instances of Constraint can have some, or none of these features.
     * So it is essential to check whether instance has corresponding features before using it.
	*/
    template<typename T>
    class Constraint {
    public:
        Constraint(FnCheck<T>&& fn_check, FnClamp<T>&& fn_clamp) :
            fn_check(std::move(fn_check)), fn_clamp(std::move(fn_clamp)) {}
        YYCC_DELETE_COPY(Constraint)
        YYCC_DEFAULT_MOVE(Constraint)

        /**
         * @brief Perform Check feature.
         * @param[in] value The valid for checking.
         * @return True if valid is okey, otherwise false.
         * @exception std::logic_error Raised if this feature is not supported.
         */
        bool check(const T& value) const {
            if (!support_check()) {
                throw std::logic_error("this Constraint do not support check operation");
            } else {
                return fn_check(value);
            }
        }
        /**
         * @brief Perform Clamp feature.
         * @param[in] value The valid for clamping.
         * @return The result after clamping.
         * @exception std::logic_error Raised if this feature is not supported.
         */
        T clamp(const T& value) const {
            if (!support_clamp()) {
                throw std::logic_error("this Constraint do not support clamp operation");
            } else {
                return fn_clamp(value);
            }
        }

        /// @brief Check whether this Constraint support Check feature.
        /// @return True if it support, otherwise false.
        bool support_check() const noexcept { return this->fn_check != nullptr; }
        /// @brief Check whether this Constraint support Clamp feature.
        /// @return True if it support, otherwise false.
        bool support_clamp() const noexcept { return this->fn_clamp != nullptr; }

    private:
        /// @brief Pointer to Check feature function.
        FnCheck<T> fn_check;
        /// @brief Pointer to Clamp feature function.
        FnClamp<T> fn_clamp;
    };

} // namespace yycc::core::constraint
