#pragma once
#include "YYCCInternal.hpp"

#include <initializer_list>
#include <type_traits>

/**
 * @brief The namespace for convenient C++ enum class logic operations.
 * @details
 * C++ enum class statement is a modern way to declare enum in C++.
 * But it lack essential logic operations which is commonly used by programmer.
 * So we create this helper to resolve this issue.
*/
namespace YYCC::EnumHelper {

	//// Reference: 
	//// Enum operator overload: https://stackoverflow.com/a/71107019
	//// Constexpr operator overload: https://stackoverflow.com/a/17746099

	//template<typename TEnum, std::enable_if_t<std::is_enum_v<TEnum>, int> = 0>
	//inline constexpr TEnum operator|(TEnum lhs, TEnum rhs) {
	//	using ut = std::underlying_type_t<TEnum>;
	//	return static_cast<TEnum>(static_cast<ut>(lhs) | static_cast<ut>(rhs));
	//}
	//template<typename TEnum, std::enable_if_t<std::is_enum_v<TEnum>, int> = 0>
	//inline constexpr TEnum operator|=(TEnum& lhs, TEnum rhs) {
	//	using ut = std::underlying_type_t<TEnum>;
	//	lhs = lhs | rhs;
	//	return lhs;
	//}
	//
	//template<typename TEnum, std::enable_if_t<std::is_enum_v<TEnum>, int> = 0>
	//inline constexpr TEnum operator&(TEnum lhs, TEnum rhs) {
	//	using ut = std::underlying_type_t<TEnum>;
	//	return static_cast<TEnum>(static_cast<ut>(lhs) & static_cast<ut>(rhs));
	//}
	//template<typename TEnum, std::enable_if_t<std::is_enum_v<TEnum>, int> = 0>
	//inline constexpr TEnum operator&=(TEnum& lhs, TEnum rhs) {
	//	using ut = std::underlying_type_t<TEnum>;
	//	lhs = lhs & rhs;
	//	return lhs;
	//}
	//
	//template<typename TEnum, std::enable_if_t<std::is_enum_v<TEnum>, int> = 0>
	//inline constexpr TEnum operator^(TEnum lhs, TEnum rhs) {
	//	using ut = std::underlying_type_t<TEnum>;
	//	return static_cast<TEnum>(static_cast<ut>(lhs) ^ static_cast<ut>(rhs));
	//}
	//template<typename TEnum, std::enable_if_t<std::is_enum_v<TEnum>, int> = 0>
	//inline constexpr TEnum operator^=(TEnum& lhs, TEnum rhs) {
	//	using ut = std::underlying_type_t<TEnum>;
	//	lhs = lhs ^ rhs;
	//	return lhs;
	//}

	//template<typename TEnum, std::enable_if_t<std::is_enum_v<TEnum>, int> = 0>
	//inline constexpr TEnum operator~(TEnum lhs) {
	//	using ut = std::underlying_type_t<TEnum>;
	//	return static_cast<TEnum>(~(static_cast<ut>(lhs)));
	//}
	//
	//template<typename TEnum, std::enable_if_t<std::is_enum_v<TEnum>, int> = 0>
	//inline constexpr bool operator bool(TEnum lhs) {
	//	using ut = std::underlying_type_t<TEnum>;
	//	return static_cast<bool>(static_cast<ut>(lhs));
	//}

	/**
	 * @brief The helper struct to check all given template argument are the same enum type.
	 * @tparam TEnum The template parameter to be checked (first one).
	 * @tparam Ts The template parameter to be checked.
	*/
	template<typename TEnum, typename... Ts>
	struct all_enum_values {
	public:
		// Please note it is std::is_same, not std::is_same_v!
		// That's std::conjunction_v required.
		static constexpr bool value = std::is_enum_v<std::remove_cv_t<TEnum>> && std::conjunction_v<std::is_same<std::remove_cv_t<TEnum>, std::remove_cv_t<Ts>>...>;
	};
	/**
	 * @brief The convenient calling to all_enum_values::value to check enum template parameter.
	 * @tparam TEnum The template parameter to be checked (first one).
	 * @tparam Ts The template parameter to be checked.
	*/
	template<typename TEnum, typename... Ts>
	inline constexpr bool all_enum_values_v = all_enum_values<TEnum, Ts...>::value;

	/**
	 * @brief Merge given enum flags like performing <TT>e1 | e2 | ... | en</TT>
	 * @tparam TEnum Enum type for processing.
	 * @param[in] val The first enum flag to be merged.
	 * @param[in] val_left Left enum flags to be merged.
	 * @return The merged enum flag.
	 * @remarks
	 * This function use recursive expansion to get final merge result.
	 * So there is no difference of each arguments.
	 * We independ first argument just served for expansion.
	*/
	template<typename TEnum, typename... Ts, std::enable_if_t<all_enum_values_v<TEnum, Ts...>, int> = 0>
	constexpr TEnum Merge(TEnum val, Ts... val_left) {
		using ut = std::underlying_type_t<TEnum>;
		ut result = static_cast<ut>(val);
		if constexpr (sizeof...(val_left) > 0) {
			result |= static_cast<ut>(Merge(val_left...));
		}
		return static_cast<TEnum>(result);
	}

	/**
	 * @brief Reverse given enum flags like performing <TT>~(e)</TT>
	 * @tparam TEnum Enum type for processing.
	 * @param[in] e The list of enum flags to be inversed.
	 * @return The inversed enum flag.
	*/
	template<typename TEnum, std::enable_if_t<std::is_enum_v<TEnum>, int> = 0>
	constexpr TEnum Invert(TEnum e) {
		using ut = std::underlying_type_t<TEnum>;
		return static_cast<TEnum>(~(static_cast<ut>(e)));
	}

	/**
	 * @brief Use specified enum flag to mask given enum flag like performing <TT>e1 &= e2</TT>
	 * @tparam TEnum Enum type for processing.
	 * @param[in,out] e1 The enum flags to be masked.
	 * @param[in] e2 The mask enum flag.
	*/
	template<typename TEnum, std::enable_if_t<std::is_enum_v<TEnum>, int> = 0>
	constexpr void Mask(TEnum& e1, TEnum e2) {
		using ut = std::underlying_type_t<TEnum>;
		e1 = static_cast<TEnum>(static_cast<ut>(e1) & static_cast<ut>(e2));
	}

	/**
	 * @brief Add multiple enum flags to given enum flag like performing <TT>e1 |= (e2 | e3 | ... | en)</TT>
	 * @tparam TEnum Enum type for processing.
	 * @param[in,out] e1 The enum flag which flags add on.
	 * @param[in] vals The enum flag to be added.
	*/
	template<typename TEnum, typename... Ts, std::enable_if_t<all_enum_values_v<TEnum, Ts...>, int> = 0>
	constexpr void Add(TEnum& e1, Ts... vals) {
		using ut = std::underlying_type_t<TEnum>;
		e1 = static_cast<TEnum>(static_cast<ut>(e1) | static_cast<ut>(Merge(vals...)));
	}

	/**
	 * @brief Remove multiple enum flags from given enum flag like performing <TT>e1 &= ~(e2 | e3 | ... | en)</TT>
	 * @tparam TEnum Enum type for processing.
	 * @param[in,out] e1 The enum flag which flags removed from.
	 * @param[in] vals The enum flag to be removed.
	*/
	template<typename TEnum, typename... Ts, std::enable_if_t<all_enum_values_v<TEnum>, int> = 0>
	constexpr void Remove(TEnum& e1, Ts... vals) {
		using ut = std::underlying_type_t<TEnum>;
		e1 = static_cast<TEnum>(static_cast<ut>(e1) & static_cast<ut>(Invert(Merge(vals...))));
	}

	/**
	 * @brief Check whether given enum flag has any of specified multiple enum flags (OR) like performing <TT>bool(e1 & (e2 | e3 | ... | en))</TT>
	 * @tparam TEnum Enum type for processing.
	 * @param[in] e1 The enum flag where we check.
	 * @param[in] vals The enum flags for checking.
	 * @return True if it has any of given flags (OR), otherwise false.
	*/
	template<typename TEnum, typename... Ts, std::enable_if_t<std::is_enum_v<TEnum>, int> = 0>
	constexpr bool Has(TEnum e1, Ts... vals) {
		using ut = std::underlying_type_t<TEnum>;
		return static_cast<bool>(static_cast<ut>(e1) & static_cast<ut>(Merge(vals...)));
	}
	
	/**
	 * @brief Cast given enum flags to its equvalent boolean value like performing <TT>bool(e)</TT>
	 * @tparam TEnum Enum type for processing.
	 * @param e The enum flags to be cast.
	 * @return The equvalent bool value of given enum flag.
	*/
	template<typename TEnum, std::enable_if_t<std::is_enum_v<TEnum>, int> = 0>
	constexpr bool Bool(TEnum e) {
		using ut = std::underlying_type_t<TEnum>;
		return static_cast<bool>(static_cast<ut>(e));
	}

}
