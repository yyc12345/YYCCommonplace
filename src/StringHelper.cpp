#include "StringHelper.hpp"
#include "EncodingHelper.hpp"
#include <algorithm>

namespace YYCC::StringHelper {

#pragma region Printf VPrintf

	bool Printf(yycc_u8string& strl, const yycc_char8_t* format, ...) {
		va_list argptr;
		va_start(argptr, format);
		bool ret = VPrintf(strl, format, argptr);
		va_end(argptr);
		return ret;
	}

	bool VPrintf(yycc_u8string& strl, const yycc_char8_t* format, va_list argptr) {
		va_list args1;
		va_copy(args1, argptr);
		va_list args2;
		va_copy(args2, argptr);

		// the return value is desired char count without NULL terminal.
		// minus number means error
		int count = std::vsnprintf(
			nullptr, 
			0, 
			EncodingHelper::ToOrdinary(format), 
			args1
		);
		if (count < 0) {
			// invalid length returned by vsnprintf.
			return false;
		}
		va_end(args1);

		// resize std::string to desired count.
		// and pass its length + 1 to std::vsnprintf,
		// because std::vsnprintf only can write "buf_size - 1" chars with a trailing NULL.
		// however std::vsnprintf already have a trailing NULL, so we plus 1 for it.
		strl.resize(count);
		int write_result = std::vsnprintf(
			EncodingHelper::ToOrdinary(strl.data()),
			strl.size() + 1, 
			EncodingHelper::ToOrdinary(format), 
			args2
		);
		va_end(args2);

		if (write_result < 0 || write_result > count) {
			// invalid write result in vsnprintf.
			return false;
		}

		return true;
	}

	yycc_u8string Printf(const yycc_char8_t* format, ...) {
		yycc_u8string ret;

		va_list argptr;
		va_start(argptr, format);
		VPrintf(ret, format, argptr);
		va_end(argptr);

		return ret;
	}

	yycc_u8string VPrintf(const yycc_char8_t* format, va_list argptr) {
		yycc_u8string ret;

		va_list argcpy;
		va_copy(argcpy, argptr);
		VPrintf(ret, format, argcpy);
		va_end(argcpy);

		return ret;
	}

#pragma endregion

#pragma region Replace

	void Replace(yycc_u8string& strl, const yycc_char8_t* _from_strl, const yycc_char8_t* _to_strl)  {
		// Reference: https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
		
		// check requirements
		// from string and to string should not be nullptr.
		if (_from_strl == nullptr || _to_strl == nullptr) return;
		// from string should not be empty
		yycc_u8string from_strl(_from_strl);
		yycc_u8string to_strl(_to_strl);
		if (from_strl.empty()) return;

		// start replace one by one
		size_t start_pos = 0;
		while ((start_pos = strl.find(from_strl, start_pos)) != yycc_u8string::npos) {
			strl.replace(start_pos, from_strl.size(), to_strl);
			start_pos += to_strl.size(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
	}

	yycc_u8string Replace(const yycc_char8_t* _strl, const yycc_char8_t* _from_strl, const yycc_char8_t* _to_strl) {
		// prepare result
		yycc_u8string strl;
		// if given string is not nullptr, assign it and process it.
		if (_strl != nullptr) {
			strl = _strl;
			Replace(strl, _from_strl, _to_strl);
		}
		// return value
		return strl;
	}

#pragma endregion

#pragma region Join

	yycc_u8string Join(JoinDataProvider fct_data, const yycc_char8_t* decilmer) {
		yycc_u8string ret;
		bool is_first = true;
		yycc_u8string_view element;

		// fetch element
		while (fct_data(element)) {
			// insert decilmer
			if (is_first) is_first = false;
			else {
				// only insert non-nullptr decilmer.
				if (decilmer != nullptr)
					ret.append(decilmer);
			}

			// insert element if it is not empty
			if (!element.empty())
				ret.append(element);
		}

		return ret;
	}

	yycc_u8string Join(const std::vector<yycc_u8string>& data, const yycc_char8_t* decilmer, bool reversed) {
		if (reversed) {
			auto iter = data.crbegin();
			auto stop = data.crend();
			return Join([&iter, &stop](yycc_u8string_view& view) -> bool {
				// if we reach tail, return false
				if (iter == stop) return false;
				// otherwise fetch data, inc iterator and return.
				view = *iter;
				++iter;
				return true;
				}, decilmer);
		} else {
			auto iter = data.cbegin();
			auto stop = data.cend();
			return Join([&iter, &stop](yycc_u8string_view& view) -> bool {
				// if we reach tail, return nullptr
				if (iter == stop) return false;
				// otherwise fetch data, inc iterator and return.
				view = *iter;
				++iter;
				return true;
				}, decilmer);
		}
	}

#pragma endregion

#pragma region Upper Lower

	template<bool bIsToLower>
	static void GeneralStringLowerUpper(yycc_u8string& strl) {
		// References:
		// https://en.cppreference.com/w/cpp/algorithm/transform
		// https://en.cppreference.com/w/cpp/string/byte/tolower
		std::transform(
			strl.cbegin(), strl.cend(), strl.begin(),
			[](unsigned char c) -> char {
				if constexpr (bIsToLower) return std::tolower(c);
				else return std::toupper(c);
			}
		);
	}

	yycc_u8string Lower(const yycc_char8_t* strl) {
		yycc_u8string ret;
		if (strl == nullptr) return ret;
		else ret = strl;
		Lower(ret);
		return ret;
	}

	void Lower(yycc_u8string& strl) {
		GeneralStringLowerUpper<true>(strl);
	}

	yycc_u8string Upper(const yycc_char8_t* strl) {
		// same as Lower, just replace char transform function.
		yycc_u8string ret;
		if (strl == nullptr) return ret;
		else ret = strl;
		Upper(ret);
		return ret;
	}

	void Upper(yycc_u8string& strl) {
		GeneralStringLowerUpper<false>(strl);
	}

#pragma endregion

#pragma region Split

	std::vector<yycc_u8string> Split(const yycc_u8string_view& strl, const yycc_char8_t* _decilmer) {
		// call split view
		auto view_result = SplitView(strl, _decilmer);

		// copy string view result to string
		std::vector<yycc_u8string> elems;
		for (const auto& strl_view : view_result) {
			elems.emplace_back(yycc_u8string(strl_view));
		}
		// return copied result
		return elems;
	}

	std::vector<yycc_u8string_view> SplitView(const yycc_u8string_view& strl, const yycc_char8_t* _decilmer) {
		// Reference: 
		// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
		
		// prepare return value
		std::vector<yycc_u8string_view> elems;

		// if string need to be splitted is empty, return original string (empty item).
		// if decilmer is nullptr, or decilmer is zero length, return original string.
		yycc_u8string decilmer;
		if (strl.empty() || _decilmer == nullptr || (decilmer = _decilmer, decilmer.empty())) {
			elems.emplace_back(strl);
			return elems;
		}

		// start spliting
		std::size_t previous = 0, current;
		while ((current = strl.find(decilmer.c_str(), previous)) != yycc_u8string::npos) {
			elems.emplace_back(strl.substr(previous, current - previous));
			previous = current + decilmer.size();
		}
		// try insert last part but prevent possible out of range exception
		if (previous <= strl.size()) {
			elems.emplace_back(strl.substr(previous));
		}
		return elems;
	}

#pragma endregion


}
