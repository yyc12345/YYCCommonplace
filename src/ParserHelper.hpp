#pragma once
#include "YYCCInternal.hpp"

#include "EncodingHelper.hpp"
#include <string>
#include <cinttypes>
#include <type_traits>
#include <stdexcept>
#include <charconv>
#include <array>

/**
 * @brief The helper involving convertion between arithmetic types (integral, floating point and bool) and string
 * @details
 * See also \ref parser_helper.
*/
namespace YYCC::ParserHelper {

	// Reference: https://zh.cppreference.com/w/cpp/utility/from_chars

	/**
	 * @brief Try parsing given string to floating point types.
	 * @tparam _Ty The type derived from floating point type.
	 * @param[in] strl The string need to be parsed.
	 * @param[out] num 
	 * The variable receiving result.
	 * There is no guarantee that the content is not modified when parsing failed.
	 * @return True if success, otherwise false.
	*/
	template<typename _Ty, std::enable_if_t<std::is_floating_point_v<_Ty>, int> = 0>
	bool TryParse(const yycc_u8string_view& strl, _Ty& num) {
		auto [ptr, ec] = std::from_chars(
			EncodingHelper::ToOrdinary(strl.data()), 
			EncodingHelper::ToOrdinary(strl.data() + strl.size()), 
			num, std::chars_format::general
		);
		if (ec == std::errc()) {
			// check whether the full string is matched
			return ptr == EncodingHelper::ToOrdinary(strl.data() + strl.size());
		} else if (ec == std::errc::invalid_argument) {
			// given string is invalid
			return false;
		} else if (ec == std::errc::result_out_of_range) {
			// given string is out of range
			return false;
		} else {
			// unreachable
			throw std::runtime_error("unreachable code.");
		}
	}
	/**
	 * @brief Try parsing given string to integral types.
	 * @tparam _Ty The type derived from integral type.
	 * @param[in] strl The string need to be parsed.
	 * @param[out] num 
	 * The variable receiving result.
	 * There is no guarantee that the content is not modified when parsing failed.
	 * @param[in] base integer base to use: a value between 2 and 36 (inclusive).
	 * @return True if success, otherwise false.
	*/
	template<typename _Ty, std::enable_if_t<std::is_integral_v<_Ty> && !std::is_same_v<_Ty, bool>, int> = 0>
	bool TryParse(const yycc_u8string_view& strl, _Ty& num, int base = 10) {
		auto [ptr, ec] = std::from_chars(
			EncodingHelper::ToOrdinary(strl.data()), 
			EncodingHelper::ToOrdinary(strl.data() + strl.size()), 
			num, base
		);
		if (ec == std::errc()) {
			// check whether the full string is matched
			return ptr == EncodingHelper::ToOrdinary(strl.data() + strl.size());
		} else if (ec == std::errc::invalid_argument) {
			// given string is invalid
			return false;
		} else if (ec == std::errc::result_out_of_range) {
			// given string is out of range
			return false;
		} else {
			// unreachable
			throw std::runtime_error("unreachable code.");
		}
	}
	/**
	 * @brief Try parsing given string to bool types.
	 * @tparam _Ty The type derived from bool type.
	 * @param[in] strl The string need to be parsed ("true" or "false").
	 * @param[out] num 
	 * The variable receiving result.
	 * There is no guarantee that the content is not modified when parsing failed.
	 * @return True if success, otherwise false.
	*/
	template<typename _Ty, std::enable_if_t<std::is_same_v<_Ty, bool>, int> = 0>
	bool TryParse(const yycc_u8string_view& strl, _Ty& num) {
		if (strl == YYCC_U8("true")) num = true;
		else if (strl == YYCC_U8("false")) num = false;
		else return false;
		return true;
	}

	/**
	 * @brief Parse given string to arithmetic types.
	 * @tparam _Ty The type derived from arithmetic type.
	 * @param[in] strl The string need to be parsed.
	 * @return
	 * The parsing result.
	 * There is no guarantee about the content of this return value when parsing failed.
	 * It may be any possible value but usually is its default value.
	*/
	template<typename _Ty, std::enable_if_t<std::is_arithmetic_v<_Ty>, int> = 0>
	_Ty Parse(const yycc_u8string_view& strl) {
		_Ty ret;
		TryParse(strl, ret);
		return ret;
	}

	// Reference: https://en.cppreference.com/w/cpp/utility/to_chars

	/**
	 * @brief Return a string version of given arithmetic value.
	 * @tparam _Ty The type derived from arithmetic type.
	 * @param[in] num The value getting string version.
	 * @return The string version of given value.
	*/
	template<typename _Ty, std::enable_if_t<std::is_arithmetic_v<_Ty> && !std::is_same_v<_Ty, bool>, int> = 0>
	yycc_u8string ToString(_Ty num) {
		std::array<yycc_char8_t, 64> buffer;
		auto [ptr, ec] = std::to_chars(
			EncodingHelper::ToOrdinary(buffer.data()), 
			EncodingHelper::ToOrdinary(buffer.data() + buffer.size()), 
			num
		);
		if (ec == std::errc()) {
			return yycc_u8string(buffer.data(), EncodingHelper::ToUTF8(ptr) - buffer.data());
		} else if (ec == std::errc::value_too_large) {
			// too short buffer
			// this should not happened
			throw std::out_of_range("ToString() buffer is not sufficient.");
		} else {
			// unreachable
			throw std::runtime_error("unreachable code.");
		}
	}
	/**
	 * @brief Return a string version of given bool value.
	 * @tparam _Ty The type derived from bool type.
	 * @param[in] num The value getting string version.
	 * @return The string version of given value ("true" or "false").
	*/
	template<typename _Ty, std::enable_if_t<std::is_same_v<_Ty, bool>, int> = 0>
	yycc_u8string ToString(_Ty num) {
		if (num) return yycc_u8string(YYCC_U8("true"));
		else return yycc_u8string(YYCC_U8("false"));
	}

}
