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
	bool TryParse(const std::string& strl, _Ty& num) {
		auto [ptr, ec] = std::from_chars(strl.c_str(), strl.c_str() + strl.size(), num, std::chars_format::general);
		if (ec == std::errc()) {
			// check whether the full string is matched
			return ptr == strl.c_str() + strl.size();
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
	bool TryParse(const std::string& strl, _Ty& num, int base = 10) {
		auto [ptr, ec] = std::from_chars(strl.c_str(), strl.c_str() + strl.size(), num, base);
		if (ec == std::errc()) {
			// check whether the full string is matched
			return ptr == strl.c_str() + strl.size();
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
	bool TryParse(const std::string& strl, _Ty& num) {
		if (strl == "true") num = true;
		else if (strl == "false") num = false;
		else return false;
		return true;
	}

	template<typename _Ty, std::enable_if_t<std::is_arithmetic_v<_Ty>, int> = 0>
	_Ty Parse(const std::string& strl) {
		_Ty ret;
		TryParse(strl, ret);
		return ret;
	}

	// Reference: https://en.cppreference.com/w/cpp/utility/to_chars

	template<typename _Ty, std::enable_if_t<std::is_arithmetic_v<_Ty> && !std::is_same_v<_Ty, bool>, int> = 0>
	std::string ToString(_Ty num) {
		std::array<char, 64> buffer;
		auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), num);
		if (ec == std::errc()) {
			return std::string(buffer.data(), ptr - buffer.data());
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
	std::string ToString(_Ty num) {
		if (num) return std::string("true");
		else return std::string("false");
	}

}
