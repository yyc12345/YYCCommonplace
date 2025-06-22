#include "op.hpp"

#include <algorithm>
#include "reinterpret.hpp"

#define NS_YYCC_STRING ::yycc::string
#define NS_YYCC_STRING_REINTERPRET ::yycc::string::reinterpret

namespace yycc::string::op {

#pragma region Printf VPrintf

	bool printf(NS_YYCC_STRING::u8string& strl, const NS_YYCC_STRING::u8char* format, ...) {
		va_list argptr;
		va_start(argptr, format);
		bool ret = vprintf(strl, format, argptr);
		va_end(argptr);
		return ret;
	}

	bool vprintf(NS_YYCC_STRING::u8string& strl, const NS_YYCC_STRING::u8char* format, va_list argptr) {
		va_list args1;
		va_copy(args1, argptr);
		va_list args2;
		va_copy(args2, argptr);

		// the return value is desired char count without NULL terminal.
		// minus number means error
		int count = std::vsnprintf(
			nullptr,
			0,
			NS_YYCC_STRING_REINTERPRET::as_ordinary(format),
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
			NS_YYCC_STRING_REINTERPRET::as_ordinary(strl.data()),
			strl.size() + 1,
			NS_YYCC_STRING_REINTERPRET::as_ordinary(format),
			args2
		);
		va_end(args2);

		if (write_result < 0 || write_result > count) {
			// invalid write result in vsnprintf.
			return false;
		}

		return true;
	}

	NS_YYCC_STRING::u8string printf(const NS_YYCC_STRING::u8char* format, ...) {
		NS_YYCC_STRING::u8string ret;

		va_list argptr;
		va_start(argptr, format);
		vprintf(ret, format, argptr);
		va_end(argptr);

		return ret;
	}

	NS_YYCC_STRING::u8string vprintf(const NS_YYCC_STRING::u8char* format, va_list argptr) {
		NS_YYCC_STRING::u8string ret;

		va_list argcpy;
		va_copy(argcpy, argptr);
		vprintf(ret, format, argcpy);
		va_end(argcpy);

		return ret;
	}

#pragma endregion

#pragma region Replace

	void replace(NS_YYCC_STRING::u8string& strl, const NS_YYCC_STRING::u8string_view& _from_strl, const NS_YYCC_STRING::u8string_view& _to_strl) {
		// Reference: https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string

		// check requirements
		// from string should not be empty
		NS_YYCC_STRING::u8string from_strl(_from_strl);
		NS_YYCC_STRING::u8string to_strl(_to_strl);
		if (from_strl.empty()) return;

		// start replace one by one
		size_t start_pos = 0;
		while ((start_pos = strl.find(from_strl, start_pos)) != NS_YYCC_STRING::u8string::npos) {
			strl.replace(start_pos, from_strl.size(), to_strl);
			start_pos += to_strl.size(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
	}

	NS_YYCC_STRING::u8string replace(const NS_YYCC_STRING::u8string_view& _strl, const NS_YYCC_STRING::u8string_view& _from_strl, const NS_YYCC_STRING::u8string_view& _to_strl) {
		// prepare result
		NS_YYCC_STRING::u8string strl(_strl);
		replace(strl, _from_strl, _to_strl);
		// return value
		return strl;
	}

#pragma endregion

#pragma region Join

	NS_YYCC_STRING::u8string join(JoinDataProvider fct_data, const NS_YYCC_STRING::u8string_view& delimiter) {
		NS_YYCC_STRING::u8string ret;
		bool is_first = true;
		NS_YYCC_STRING::u8string_view element;

		// fetch element
		while (fct_data(element)) {
			// insert delimiter
			if (is_first) is_first = false;
			else {
				// append delimiter.
				ret.append(delimiter);
			}

			// insert element if it is not empty
			if (!element.empty())
				ret.append(element);
		}

		return ret;
	}

#pragma endregion

#pragma region Upper Lower

	template<bool BIsToLower>
	static void generic_lower_upper(NS_YYCC_STRING::u8string& strl) {
		// References:
		// https://en.cppreference.com/w/cpp/algorithm/transform
		// https://en.cppreference.com/w/cpp/string/byte/tolower
		std::transform(
			strl.cbegin(), strl.cend(), strl.begin(),
			[](unsigned char c) -> char {
				if constexpr (BIsToLower) return std::tolower(c);
				else return std::toupper(c);
			}
		);
	}

	void lower(NS_YYCC_STRING::u8string& strl) {
		generic_lower_upper<true>(strl);
	}

	NS_YYCC_STRING::u8string lower(const NS_YYCC_STRING::u8string_view& strl) {
		NS_YYCC_STRING::u8string ret(strl);
		lower(ret);
		return ret;
	}

	void upper(NS_YYCC_STRING::u8string& strl) {
		generic_lower_upper<false>(strl);
	}

	NS_YYCC_STRING::u8string upper(const NS_YYCC_STRING::u8string_view& strl) {
		// same as Lower, just replace char transform function.
		NS_YYCC_STRING::u8string ret(strl);
		upper(ret);
		return ret;
	}

#pragma endregion

#pragma region Split

    std::vector<NS_YYCC_STRING::u8string_view> split(const NS_YYCC_STRING::u8string_view& strl, const NS_YYCC_STRING::u8string_view& _delimiter) {
        // Reference:
        // https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c

        // prepare return value
        std::vector<NS_YYCC_STRING::u8string_view> elems;

        // if string need to be splitted is empty, return original string (empty string).
        // if delimiter is empty, return original string.
        NS_YYCC_STRING::u8string delimiter(_delimiter);
        if (strl.empty() || delimiter.empty()) {
            elems.emplace_back(strl);
            return elems;
        }

        // start spliting
        std::size_t previous = 0, current;
        while ((current = strl.find(delimiter.c_str(), previous)) != NS_YYCC_STRING::u8string::npos) {
            elems.emplace_back(strl.substr(previous, current - previous));
            previous = current + delimiter.size();
        }
        // try insert last part but prevent possible out of range exception
        if (previous <= strl.size()) {
            elems.emplace_back(strl.substr(previous));
        }
        return elems;
    }

    std::vector<NS_YYCC_STRING::u8string> split_owned(const NS_YYCC_STRING::u8string_view& strl, const NS_YYCC_STRING::u8string_view& _delimiter) {
		// call split view
        auto view_result = split(strl, _delimiter);

		// copy string view result to string
		std::vector<NS_YYCC_STRING::u8string> elems;
		elems.reserve(view_result.size());
		for (const auto& strl_view : view_result) {
			elems.emplace_back(NS_YYCC_STRING::u8string(strl_view));
		}
		// return copied result
		return elems;
	}

#pragma endregion

}
