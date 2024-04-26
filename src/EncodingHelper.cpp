#include "EncodingHelper.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

namespace YYCC::EncodingHelper {

	bool WcharToChar(const wchar_t* src, std::string& dest, UINT codepage) {
		int count, write_result;

		//converter to CHAR
		count = WideCharToMultiByte(codepage, 0, src, -1, NULL, 0, NULL, NULL);
		if (count <= 0) return false;

		dest.resize(count - 1);
		write_result = WideCharToMultiByte(codepage, 0, src, -1, dest.data(), count, NULL, NULL);
		if (write_result <= 0) return false;

		return true;
	}
	bool WcharToChar(const std::wstring& src, std::string& dest, UINT codepage) {
		return WcharToChar(src.c_str(), dest, codepage);
	}
	std::string WcharToChar(const wchar_t* src, UINT codepage) {
		std::string ret;
		if (!WcharToChar(src, ret, codepage)) ret.clear();
		return ret;
	}
	std::string WcharToChar(const std::wstring& src, UINT codepage) {
		std::string ret;
		if (!WcharToChar(src.c_str(), ret, codepage)) ret.clear();
		return ret;
	}

	bool CharToWchar(const char* src, std::wstring& dest, UINT codepage) {
		int wcount, write_result;

		// convert to WCHAR
		wcount = MultiByteToWideChar(codepage, 0, src, -1, NULL, 0);
		if (wcount <= 0) return false;

		dest.resize(wcount - 1);
		write_result = MultiByteToWideChar(codepage, 0, src, -1, dest.data(), wcount);
		if (write_result <= 0) return false;

		return true;
	}
	bool CharToWchar(const std::string& src, std::wstring& dest, UINT codepage) {
		return CharToWchar(src.c_str(), dest, codepage);
	}
	std::wstring CharToWchar(const char* src, UINT codepage) {
		std::wstring ret;
		if (!CharToWchar(src, ret, codepage)) ret.clear();
		return ret;
	}
	std::wstring CharToWchar(const std::string& src, UINT codepage) {
		std::wstring ret;
		if (!CharToWchar(src.c_str(), ret, codepage)) ret.clear();
		return ret;
	}

	bool CharToChar(const char* src, std::string& dest, UINT src_codepage, UINT dest_codepage) {
		std::wstring intermediary;
		if (!CharToWchar(src, intermediary, src_codepage)) return false;
		if (!WcharToChar(intermediary, dest, dest_codepage)) return false;
		return true;
	}
	bool CharToChar(const std::string& src, std::string& dest, UINT src_codepage, UINT dest_codepage) {
		return CharToChar(src.c_str(), dest, src_codepage, dest_codepage);
	}
	std::string CharToChar(const char* src, UINT src_codepage, UINT dest_codepage) {
		std::string ret;
		if (!CharToChar(src, ret, src_codepage, dest_codepage)) ret.clear();
		return ret;
	}
	std::string CharToChar(const std::string& src, UINT src_codepage, UINT dest_codepage) {
		std::string ret;
		if (!CharToChar(src.c_str(), ret, src_codepage, dest_codepage)) ret.clear();
		return ret;
	}

}

#endif
