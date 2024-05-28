#pragma once
#include "YYCCInternal.hpp"

#include "EncodingHelper.hpp"
#include <string>
#include <cinttypes>
#include <type_traits>
#include <stdexcept>
#include <limits>

namespace YYCC::ParserHelper {

	template<class>
	constexpr bool g_AlwaysFalse = false;

	template<typename _Ty, std::enable_if_t<std::is_floating_point_v<_Ty>, int> = 0>
	bool TryParse(const std::string& strl, _Ty& num) {
		try {
			// float types
			if constexpr (std::is_same_v<_Ty, float>) {
				num = std::stof(strl, nullptr);
			} else if constexpr (std::is_same_v<_Ty, double>) {
				num = std::stod(strl, nullptr);
			} else if constexpr (std::is_same_v<_Ty, long double>) {
				num = std::stold(strl, nullptr);
			} else {
				static_assert(g_AlwaysFalse<_Ty>, "Invalid float type.");
			}
			return true;
		} catch (const std::invalid_argument&) {
			return false;
		} catch (const std::out_of_range&) {
			return false;
		}
	}
	template<typename _Ty, std::enable_if_t<std::is_integral_v<_Ty> && !std::is_same_v<_Ty, bool>, int> = 0>
	bool TryParse(const std::string& strl, _Ty& num, int base = 10) {
		try {
			// integer type
			// decide integer type
			using container_t = std::conditional_t<std::is_unsigned_v<_Ty>, unsigned long long, long long>;
			// parse it from string according to whether integer type is signed.
			container_t cache;
			if constexpr (std::is_unsigned_v<_Ty>) {
				cache = std::stoull(strl, nullptr, base);
			} else {
				cache = std::stoll(strl, nullptr, base);
			}
			// check its range
			if (cache < std::numeric_limits<_Ty>::min() || cache > std::numeric_limits<_Ty>::max())
				return false;
			num = static_cast<_Ty>(cache);
			return true;
		} catch (const std::invalid_argument&) {
			return false;
		} catch (const std::out_of_range&) {
			return false;
		}
	}
	bool TryParse(const std::string& strl, bool& num) {
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

	template<typename _Ty, std::enable_if_t<std::is_arithmetic_v<_Ty>, int> = 0>
	std::string ToString(_Ty num) {
		return std::to_string(num);
	}
	template<>
	std::string ToString<bool>(bool num) {
		if (num) return std::string("true");
		else return std::string("false");
	}

}
