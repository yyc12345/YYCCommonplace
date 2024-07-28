#pragma once
#include "YYCCInternal.hpp"

#include <functional>
#include <stdexcept>
#include <set>
#include <initializer_list>

/**
 * @brief The namespace containing constraint declaration
 * and functions generating common used constraint.
*/
namespace YYCC::Constraints {

	/**
	 * @brief The constraint applied to settings to limit its stored value.
	 * @tparam _Ty The internal data type stroed in corresponding setting.
	*/
	template<typename _Ty>
	struct Constraint {
		/// @brief Return true if value is legal, otherwise false.
		using CheckFct_t = std::function<bool(const _Ty&)>;
		/// @brief The function pointer used for checking whether given value is valid.
		CheckFct_t m_CheckFct;
		
		/**
		 * @brief Check whether this Constraint is valid for using.
		 * @return 
		 * True if this Constraint is valid, otherwise false.
		 * You should not use this Constraint if this function return false.
		*/
		bool IsValid() const {
			return m_CheckFct != nullptr;
		}
	};

	/**
	 * @brief Get constraint for arithmetic or enum values by minimum and maximum value range.
	 * @tparam _Ty An arithmetic or enum type (except bool) of underlying stored value.
	 * @param[in] min_value The minimum value of range (inclusive).
	 * @param[in] max_value The maximum value of range (inclusive).
	 * @return The generated constraint instance which can be directly applied.
	*/
	template<typename _Ty, std::enable_if_t<std::is_arithmetic_v<_Ty> && !std::is_same_v<_Ty, bool>, int> = 0>
	Constraint<_Ty> GetMinMaxRangeConstraint(_Ty min_value, _Ty max_value) {
		if (min_value > max_value)
			throw std::invalid_argument("invalid min max value for NumberRangeConstraint");
		return Constraint<_Ty> {
			[min_value, max_value](const _Ty& val) -> bool { return (val <= max_value) && (val >= min_value); }
				/*[min_value, max_value](const _Ty& val) -> _Ty { return std::clamp(val, min_value, max_value); }*/
		};
	}

	/**
	 * @brief Get constraint for enum values by enumerate all possible values.
	 * @tparam _Ty An enum type (except bool) of underlying stored value.
	 * @param[in] il An initializer list storing all possible values.
	 * @return The generated constraint instance which can be directly applied.
	*/
	template<typename _Ty, std::enable_if_t<std::is_enum_v<_Ty>, int> = 0>
	Constraint<_Ty> GetEnumEnumerationConstraint(const std::initializer_list<_Ty>& il) {
		std::set<_Ty> data(il);
		return Constraint<_Ty> {
			[data](const _Ty& val) -> bool { return data.find(val) != data.end(); }
		};
	}

	/**
	 * @brief Get constraint for string values by enumerate all possible values.
	 * @param[in] il An initializer list storing all possible values.
	 * @return The generated constraint instance which can be directly applied.
	 * @remarks
	 * Caller must make sure that the string view passed in initializer list is valid until this Constraint life time gone.
	 * Becasue this generator will not copy your given string view into string.
	*/
	Constraint<yycc_u8string_view> GetStringEnumerationConstraint(const std::initializer_list<yycc_u8string_view>& il) {
		std::set<yycc_u8string_view> data(il);
		return Constraint<yycc_u8string_view> {
			[data](const yycc_u8string_view& val) -> bool { return data.find(val) != data.end(); }
		};
	}

}

