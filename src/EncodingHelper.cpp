#include "EncodingHelper.hpp"

#include <cuchar>

namespace YYCC::EncodingHelper {

#if YYCC_OS == YYCC_OS_WINDOWS

	bool WcharToChar(const wchar_t* src, std::string& dest, UINT codepage) {
		int count, write_result;

		//converter to CHAR
		count = WideCharToMultiByte(codepage, 0, reinterpret_cast<LPCWCH>(src), -1, NULL, 0, NULL, NULL);
		if (count <= 0) return false;

		dest.resize(count - 1);
		write_result = WideCharToMultiByte(codepage, 0, reinterpret_cast<LPCWCH>(src), -1, reinterpret_cast<LPSTR>(dest.data()), count, NULL, NULL);
		if (write_result <= 0) return false;

		return true;
	}
	bool WcharToUTF8(const wchar_t* src, std::string& dest) {
		return WcharToChar(src, dest, CP_UTF8);
	}
	std::string WcharToChar(const wchar_t* src, UINT codepage) {
		std::string ret;
		if (!WcharToChar(src, ret, codepage)) ret.clear();
		return ret;
	}
	std::string WcharToUTF8(const wchar_t* src) {
		return WcharToChar(src, CP_UTF8);
	}

	bool CharToWchar(const char* src, std::wstring& dest, UINT codepage) {
		int wcount, write_result;

		// convert to WCHAR
		wcount = MultiByteToWideChar(codepage, 0, reinterpret_cast<LPCCH>(src), -1, NULL, 0);
		if (wcount <= 0) return false;

		dest.resize(wcount - 1);
		write_result = MultiByteToWideChar(codepage, 0, reinterpret_cast<LPCCH>(src), -1, reinterpret_cast<LPWSTR>(dest.data()), wcount);
		if (write_result <= 0) return false;

		return true;
	}
	bool UTF8ToWchar(const char* src, std::wstring& dest) {
		return CharToWchar(src, dest, CP_UTF8);
	}
	std::wstring CharToWchar(const char* src, UINT codepage) {
		std::wstring ret;
		if (!CharToWchar(src, ret, codepage)) ret.clear();
		return ret;
	}
	std::wstring UTF8ToWchar(const char* src) {
		return CharToWchar(src, CP_UTF8);
	}

	bool CharToChar(const char* src, std::string& dest, UINT src_codepage, UINT dest_codepage) {
		std::wstring intermediary;
		if (!CharToWchar(src, intermediary, src_codepage)) return false;
		if (!WcharToChar(intermediary.c_str(), dest, dest_codepage)) return false;
		return true;
	}
	std::string CharToChar(const char* src, UINT src_codepage, UINT dest_codepage) {
		std::string ret;
		if (!CharToChar(src, ret, src_codepage, dest_codepage)) ret.clear();
		return ret;
	}

#endif

	template<typename _TChar, std::enable_if_t<std::is_same_v<_TChar, char16_t> || std::is_same_v<_TChar, char32_t>, int> = 0>
	static bool UTF8ToUTFOther(const char* src, std::basic_string<_TChar>& dest) {
		// Reference: 
		// https://zh.cppreference.com/w/cpp/string/multibyte/mbrtoc32
		// https://zh.cppreference.com/w/cpp/string/multibyte/mbrtoc16
		// https://learn.microsoft.com/zh-cn/cpp/c-runtime-library/reference/mbrtoc16-mbrtoc323?view=msvc-170
		// 
		// Due to the same reason introduced in UTFOtherToUTF8,
		// we use these function as convertion function.
		
		// init src string
		if (src == nullptr) return false;
		std::string src_string(src);
		// init result string
		dest.clear();

		// init essential cvt variables
		std::mbstate_t state {};
		_TChar c1632;
		const char* ptr = src_string.c_str();
		const char* end = src_string.c_str() + src_string.size() + 1;

		// start convertion
		while (true) {
			// do convertion
			size_t rc;
			if constexpr (std::is_same_v<_TChar, char16_t>) {
				rc = std::mbrtoc16(&c1632, ptr, end - ptr, &state);
			} else {
				rc = std::mbrtoc32(&c1632, ptr, end - ptr, &state);
			}
			if (!rc) break;

			// check result
			if (rc == static_cast<size_t>(-1)) {
				// encoding error, return false
				return false;
			} else if (rc == static_cast<size_t>(-2)) {
				// insufficient sequence, return false
				return false;
			} else if (rc == static_cast<size_t>(-3)) {
				// UTF16 pair case (usually is emoji, one emoji is represented by 2 UTF16)
				// 
				// only push result char but do not increase pointer
				// because this char is output from state.
				dest.push_back(c1632);
			} else {
				// normal case
				// append to result
				dest.push_back(c1632);
				// inc ptr
				ptr += rc;
			}
		}

		return true;
	}

	bool UTF8ToUTF16(const char* src, std::u16string& dest) {
		return UTF8ToUTFOther<char16_t>(src, dest);
	}
	std::u16string UTF8ToUTF16(const char* src) {
		std::u16string ret;
		if (!UTF8ToUTF16(src, ret)) ret.clear();
		return ret;
	}
	bool UTF8ToUTF32(const char* src, std::u32string& dest) {
		return UTF8ToUTFOther<char32_t>(src, dest);
	}
	std::u32string UTF8ToUTF32(const char* src) {
		std::u32string ret;
		if (!UTF8ToUTF32(src, ret)) ret.clear();
		return ret;
	}

	template<typename _TChar, std::enable_if_t<std::is_same_v<_TChar, char16_t> || std::is_same_v<_TChar, char32_t>, int> = 0>
	static bool UTFOtherToUTF8(const _TChar* src, std::string& dest) {
		// Reference:
		// https://zh.cppreference.com/w/cpp/string/multibyte/c32rtomb
		// https://zh.cppreference.com/w/cpp/string/multibyte/c16rtomb
		// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/c16rtomb-c32rtomb1?view=msvc-170
		// 
		// Due to Microsoft implementation, c16rtomb and c32rtomb 
		// always convert UTF32 and UTF16 string into UTF8 string no matter current c locale.
		// At the same time, most Linux use UTF8 as their locale.
		// So using c16rtomb and c32rtomb do the convertion from UTF32 or UTF16 to UTF8 is reasonable.

		// initialize src string
		if (src == nullptr) return false;
		std::basic_string<_TChar> src_string(src);
		// init result string
		dest.clear();

		// init essential cvt variables
		std::mbstate_t state {};
		char out[MB_LEN_MAX] {};
		for (_TChar c : src_string) {
			// do convertion
			std::size_t rc;
			if constexpr (std::is_same_v<_TChar, char16_t>) {
				rc = std::c16rtomb(out, c, &state);
			} else {
				rc = std::c32rtomb(out, c, &state);
			}
			// convertion failed
			if (rc == static_cast<size_t>(-1)) return false;
			// otherwise append result
			dest.append(out, rc);
		}

		return true;
	}

	bool UTF16ToUTF8(const char16_t* src, std::string& dest) {
		return UTFOtherToUTF8<char16_t>(src, dest);
	}
	std::string UTF16ToUTF8(const char16_t* src) {
		std::string ret;
		if (!UTF16ToUTF8(src, ret)) ret.clear();
		return ret;
	}
	bool UTF32ToUTF8(const char32_t* src, std::string& dest) {
		return UTFOtherToUTF8<char32_t>(src, dest);
	}
	std::string UTF32ToUTF8(const char32_t* src) {
		std::string ret;
		if (!UTF32ToUTF8(src, ret)) ret.clear();
		return ret;
	}

}

