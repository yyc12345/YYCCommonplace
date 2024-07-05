#pragma once
#include "YYCCInternal.hpp"

#include "EncodingHelper.hpp"
#include <string>
#include <cinttypes>
#include <type_traits>
#include <stdexcept>
#include <charconv>
#include <array>

namespace YYCC::ParserHelper {

	// Reference: https://zh.cppreference.com/w/cpp/utility/from_chars

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
	template<typename _Ty, std::enable_if_t<std::is_same_v<_Ty, bool>, int> = 0>
	bool TryParse(const yycc_u8string_view& strl, _Ty& num) {
		if (strl == YYCC_U8("true")) num = true;
		else if (strl == YYCC_U8("false")) num = false;
		else return false;
		return true;
	}

	template<typename _Ty, std::enable_if_t<std::is_arithmetic_v<_Ty>, int> = 0>
	_Ty Parse(const yycc_u8string_view& strl) {
		_Ty ret;
		TryParse(strl, ret);
		return ret;
	}

	// Reference: https://en.cppreference.com/w/cpp/utility/to_chars

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
			// this should not happend
			throw std::out_of_range("ToString() buffer is not sufficient.");
		} else {
			// unreachable
			throw std::runtime_error("unreachable code.");
		}
	}
	template<typename _Ty, std::enable_if_t<std::is_same_v<_Ty, bool>, int> = 0>
	yycc_u8string ToString(_Ty num) {
		if (num) return yycc_u8string(YYCC_U8("true"));
		else return yycc_u8string(YYCC_U8("false"));
	}

}
