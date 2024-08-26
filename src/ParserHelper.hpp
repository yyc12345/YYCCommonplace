#pragma once
#include "YYCCInternal.hpp"

#include "EncodingHelper.hpp"
#include "StringHelper.hpp"
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
	 * @param[in] fmt The floating point format used when try parsing.
	 * @return True if success, otherwise false.
	*/
	template<typename _Ty, std::enable_if_t<std::is_floating_point_v<_Ty>, int> = 0>
	bool TryParse(const yycc_u8string_view& strl, _Ty& num, std::chars_format fmt = std::chars_format::general) {
		auto [ptr, ec] = std::from_chars(
			EncodingHelper::ToOrdinary(strl.data()), 
			EncodingHelper::ToOrdinary(strl.data() + strl.size()), 
			num, fmt
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
	 * @tparam _Ty The type derived from integral type except bool type.
	 * @param[in] strl The string need to be parsed.
	 * @param[out] num 
	 * The variable receiving result.
	 * There is no guarantee that the content is not modified when parsing failed.
	 * @param[in] base Integer base to use: a value between 2 and 36 (inclusive).
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
	 * @param[in] strl The string need to be parsed ("true" or "false", case insensitive).
	 * @param[out] num 
	 * The variable receiving result.
	 * There is no guarantee that the content is not modified when parsing failed.
	 * @return True if success, otherwise false.
	*/
	template<typename _Ty, std::enable_if_t<std::is_same_v<_Ty, bool>, int> = 0>
	bool TryParse(const yycc_u8string_view& strl, _Ty& num) {
		// get lower case
		yycc_u8string lower_case(strl);
		YYCC::StringHelper::Lower(lower_case);
		// compare result
		if (strl == YYCC_U8("true")) num = true;
		else if (strl == YYCC_U8("false")) num = false;
		else return false;
		return true;
	}
	
	/**
	 * @brief Parse given string to floating point types.
	 * @tparam _Ty The type derived from floating point type.
	 * @param[in] strl The string need to be parsed.
	 * @param[in] fmt The floating point format used when try parsing.
	 * @return
	 * The parsing result.
	 * There is no guarantee about the content of this return value when parsing failed.
	 * It may be any possible value but usually is its default value.
	*/
	template<typename _Ty, std::enable_if_t<std::is_floating_point_v<_Ty>, int> = 0>
	_Ty Parse(const yycc_u8string_view& strl, std::chars_format fmt = std::chars_format::general) {
		_Ty ret;
		TryParse(strl, ret, fmt);
		return ret;
	}
	/**
	 * @brief Parse given string to integral type types.
	 * @tparam _Ty The type derived from integral type except bool type.
	 * @param[in] strl The string need to be parsed.
	 * @param[in] base Integer base to use: a value between 2 and 36 (inclusive).
	 * @return
	 * The parsing result.
	 * There is no guarantee about the content of this return value when parsing failed.
	 * It may be any possible value but usually is its default value.
	*/
	template<typename _Ty, std::enable_if_t<std::is_integral_v<_Ty> && !std::is_same_v<_Ty, bool>, int> = 0>
	_Ty Parse(const yycc_u8string_view& strl, int base = 10) {
		_Ty ret;
		TryParse(strl, ret, base);
		return ret;
	}
	/**
	 * @brief Parse given string to bool types.
	 * @tparam _Ty The type derived from bool type.
	 * @param[in] strl The string need to be parsed ("true" or "false", case insensitive).
	 * @return
	 * The parsing result.
	 * There is no guarantee about the content of this return value when parsing failed.
	 * It may be any possible value but usually is its default value.
	*/
	template<typename _Ty, std::enable_if_t<std::is_same_v<_Ty, bool>, int> = 0>
	_Ty Parse(const yycc_u8string_view& strl) {
		_Ty ret;
		TryParse(strl, ret);
		return ret;
	}

	// Reference: https://en.cppreference.com/w/cpp/utility/to_chars

	/**
	 * @brief Return the string representation of given floating point value.
	 * @tparam _Ty The type derived from floating point type.
	 * @param[in] num The value need to get string representation.
	 * @param[in] fmt The floating point format used when getting string representation.
	 * @param[in] precision The floating point precision used when getting string representation.
	 * @return The string representation of given value.
	*/
	template<typename _Ty, std::enable_if_t<std::is_floating_point_v<_Ty>, int> = 0>
	yycc_u8string ToString(_Ty num, std::chars_format fmt = std::chars_format::general, int precision = 6) {
		// default precision = 6 is gotten from: https://en.cppreference.com/w/c/io/fprintf
		std::array<yycc_char8_t, 64> buffer;
		auto [ptr, ec] = std::to_chars(
			EncodingHelper::ToOrdinary(buffer.data()), 
			EncodingHelper::ToOrdinary(buffer.data() + buffer.size()), 
			num, fmt, precision
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
	 * @brief Return the string representation of given integral value.
	 * @tparam _Ty The type derived from integral type except bool type.
	 * @param[in] num The value need to get string representation.
	 * @param[in] base Integer base used when getting string representation: a value between 2 and 36 (inclusive).
	 * @return The string representation of given value.
	*/
	template<typename _Ty, std::enable_if_t<std::is_integral_v<_Ty> && !std::is_same_v<_Ty, bool>, int> = 0>
	yycc_u8string ToString(_Ty num, int base = 10) {
		std::array<yycc_char8_t, 64> buffer;
		auto [ptr, ec] = std::to_chars(
			EncodingHelper::ToOrdinary(buffer.data()), 
			EncodingHelper::ToOrdinary(buffer.data() + buffer.size()), 
			num, base
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
	 * @brief Return the string representation of given bool value.
	 * @tparam _Ty The type derived from bool type.
	 * @param[in] num The value need to get string representation.
	 * @return The string representation of given value ("true" or "false").
	*/
	template<typename _Ty, std::enable_if_t<std::is_same_v<_Ty, bool>, int> = 0>
	yycc_u8string ToString(_Ty num) {
		if (num) return yycc_u8string(YYCC_U8("true"));
		else return yycc_u8string(YYCC_U8("false"));
	}

}
