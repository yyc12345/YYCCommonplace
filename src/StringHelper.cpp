#include "StringHelper.hpp"
#include <algorithm>

namespace YYCC::StringHelper {

	bool Printf(std::string& strl, const char* format, ...) {
		va_list argptr;
		va_start(argptr, format);
		bool ret = VPrintf(strl, format, argptr);
		va_end(argptr);
		return ret;
	}

	bool VPrintf(std::string& strl, const char* format, va_list argptr) {
		va_list args1;
		va_copy(args1, argptr);
		va_list args2;
		va_copy(args2, argptr);

		// the return value is desired char count without NULL terminal.
		// minus number means error
		int count = std::vsnprintf(nullptr, 0, format, args1);
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
		int write_result = std::vsnprintf(strl.data(), strl.size() + 1, format, args2);
		va_end(args2);

		if (write_result < 0 || write_result > count) {
			// invalid write result in vsnprintf.
			return false;
		}

		return true;
	}


	std::string Printf(const char* format, ...) {
		std::string ret;

		va_list argptr;
		va_start(argptr, format);
		VPrintf(ret, format, argptr);
		va_end(argptr);

		return ret;
	}

	std::string VPrintf(const char* format, va_list argptr) {
		std::string ret;

		va_list argcpy;
		va_copy(argcpy, argptr);
		VPrintf(ret, format, argcpy);
		va_end(argcpy);

		return ret;
	}

	std::string Join(JoinDataProvider fct_data, const char* decilmer) {
		std::string ret;
		bool is_first = true;
		const char* element;

		// fetch element
		while ((element = fct_data()) != nullptr) {
			// insert decilmer
			if (is_first) is_first = false;
			else {
				// only insert non-nullptr decilmer.
				if (decilmer != nullptr)
					ret.append(decilmer);
			}

			// insert element
			ret.append(element);
		}

		return ret;
	}

	std::string Join(const std::vector<std::string>& data, const char* decilmer, bool reversed) {
		if (reversed) {
			auto iter = data.crbegin();
			auto stop = data.crend();
			return Join([&iter, &stop]() -> const char* {
				// if we reach tail, return nullptr
				if (iter == stop) return nullptr;
				// otherwise fetch data, inc iterator and return.
				const char* ret = iter->c_str();
				++iter;
				return ret;
			}, decilmer);
		} else {
			auto iter = data.cbegin();
			auto stop = data.cend();
			return Join([&iter, &stop]() -> const char* {
				// if we reach tail, return nullptr
				if (iter == stop) return nullptr;
				// otherwise fetch data, inc iterator and return.
				const char* ret = iter->c_str();
				++iter;
				return ret;
			}, decilmer);
		}
	}


	template<bool bIsToLower>
	void GeneralStringLowerUpper(std::string& strl) {
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

	std::string Lower(const char* strl) {
		std::string ret;
		if (strl == nullptr) return ret;
		else ret = strl;
		Lower(ret);
		return ret;
	}

	void Lower(std::string& strl) {
		GeneralStringLowerUpper<true>(strl);
	}

	std::string Upper(const char* strl) {
		// same as Lower, just replace char transform function.
		std::string ret;
		if (strl == nullptr) return ret;
		else ret = strl;
		Upper(ret);
		return ret;
	}

	void Upper(std::string& strl) {
		GeneralStringLowerUpper<false>(strl);
	}

	std::vector<std::string> Split(const char* _strl, const char* _decilmer) {
		std::vector<std::string> elems;
		
		// check whether string and decilmer are nullptr
		std::string strl, decilmer;
		if (_strl == nullptr || _decilmer == nullptr) 
			return elems;
		else {
			strl = _strl;
			decilmer = _decilmer;
		}
		// if no decilmer, return the whole string as the only one element of result.
		if (decilmer.empty()) {
			elems.push_back(strl);
			return elems;
		}
		
		// start spliting
		std::size_t previous = 0;
		std::size_t current = strl.find(decilmer.c_str());
		while (current != std::string::npos) {
			if (current >= previous) {
				elems.push_back(strl.substr(previous, current - previous));
			}
			previous = current + decilmer.size();
			current = strl.find(decilmer, previous);
		}
		if (previous != strl.size()) {
			elems.push_back(strl.substr(previous));
		}
		return elems;
	}

}
