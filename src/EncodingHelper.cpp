#include "EncodingHelper.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

namespace YYCC::EncodingHelper {

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

}

#endif
