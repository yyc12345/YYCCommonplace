#include "EncodingHelper.hpp"

#include <locale>

namespace YYCC::EncodingHelper {

	/* Define some assistant macros for easy writing. */

#define CONVFCT_TYPE2(fct_name, src_char_type, dst_char_type, ...) if (src == nullptr) return false; \
std::basic_string_view<src_char_type> cache(src); \
return fct_name(cache, dst, ##__VA_ARGS__);

#define CONVFCT_TYPE3(fct_name, src_char_type, dst_char_type, ...) std::basic_string<dst_char_type> ret; \
if (!fct_name(src, ret, ##__VA_ARGS__)) ret.clear(); \
return ret;
	
#define CONVFCT_TYPE4(fct_name, src_char_type, dst_char_type, ...) std::basic_string<dst_char_type> ret; \
if (!fct_name(src, ret, ##__VA_ARGS__)) ret.clear(); \
return ret;


#if YYCC_OS == YYCC_OS_WINDOWS

#pragma region WcharToChar
	
	bool WcharToChar(const std::wstring_view& src, std::string& dst, UINT code_page) {
		// if src is empty, direct output
		if (src.empty()) {
			dst.clear();
			return true;
		}

		// init WideCharToMultiByte used variables
		// setup src pointer
		LPCWCH lpWideCharStr =  reinterpret_cast<LPCWCH>(src.data());
		// check whether source string is too large.
		size_t cSrcSize = src.size();
		if (cSrcSize > std::numeric_limits<int>::max()) return false;
		int cchWideChar = static_cast<int>(src.size());

		// do convertion
		// do a dry-run first to fetch desired size.
		int desired_size = WideCharToMultiByte(code_page, 0, lpWideCharStr, cchWideChar, NULL, 0, NULL, NULL);
		if (desired_size <= 0) return false;
		// resize dest for receiving result
		dst.resize(static_cast<size_t>(desired_size));
		// do real convertion
		int write_result = WideCharToMultiByte(code_page, 0, lpWideCharStr, cchWideChar, reinterpret_cast<LPSTR>(dst.data()), desired_size, NULL, NULL);
		if (write_result <= 0) return false;

		return true;
	}
	bool WcharToChar(const wchar_t* src, std::string& dst, UINT code_page) {
		CONVFCT_TYPE2(WcharToChar, wchar_t, char, code_page);
	}
	std::string WcharToChar(const std::wstring_view& src, UINT code_page) {
		CONVFCT_TYPE3(WcharToChar, wchar_t, char, code_page);
	}
	std::string WcharToChar(const wchar_t* src, UINT code_page) {
		CONVFCT_TYPE4(WcharToChar, wchar_t, char, code_page);
	}

#pragma endregion
	
#pragma region CharToWchar

	bool CharToWchar(const std::string_view& src, std::wstring& dst, UINT code_page) {
		// if src is empty, direct output
		if (src.empty()) {
			dst.clear();
			return true;
		}

		// init WideCharToMultiByte used variables
		// setup src pointer
		LPCCH lpMultiByteStr = reinterpret_cast<LPCCH>(src.data());
		// check whether source string is too large.
		size_t cSrcSize = src.size();
		if (cSrcSize > std::numeric_limits<int>::max()) return false;
		int cbMultiByte = static_cast<int>(src.size());
		
		// do convertion
		// do a dry-run first to fetch desired size.
		int desired_size = MultiByteToWideChar(code_page, 0, lpMultiByteStr, cbMultiByte, NULL, 0);
		if (desired_size <= 0) return false;
		// resize dest for receiving result
		dst.resize(static_cast<size_t>(desired_size));
		// do real convertion
		int write_result = MultiByteToWideChar(code_page, 0, lpMultiByteStr, cbMultiByte, reinterpret_cast<LPWSTR>(dst.data()), desired_size);
		if (write_result <= 0) return false;

		return true;
	}
	bool CharToWchar(const char* src, std::wstring& dst, UINT code_page) {
		CONVFCT_TYPE2(CharToWchar, char, wchar_t, code_page);
	}
	std::wstring CharToWchar(const std::string_view& src, UINT code_page) {
		CONVFCT_TYPE3(CharToWchar, char, wchar_t, code_page);
	}
	std::wstring CharToWchar(const char* src, UINT code_page) {
		CONVFCT_TYPE4(CharToWchar, char, wchar_t, code_page);
	}

#pragma endregion

#pragma region CharToChar
	
	bool CharToChar(const std::string_view& src, std::string& dst, UINT src_code_page, UINT dst_code_page) {
		std::wstring intermediary;
		if (!CharToWchar(src, intermediary, src_code_page)) return false;
		if (!WcharToChar(intermediary, dst, dst_code_page)) return false;
		return true;
	}
	bool CharToChar(const char* src, std::string& dst, UINT src_code_page, UINT dst_code_page) {
		CONVFCT_TYPE2(CharToChar, char, char, src_code_page, dst_code_page);
	}
	std::string CharToChar(const std::string_view& src, UINT src_code_page, UINT dst_code_page) {
		CONVFCT_TYPE3(CharToChar, char, char, src_code_page, dst_code_page);
	}
	std::string CharToChar(const char* src, UINT src_code_page, UINT dst_code_page) {
		CONVFCT_TYPE4(CharToChar, char, char, src_code_page, dst_code_page);
	}

#pragma endregion

#pragma region WcharToUTF8
	
	bool WcharToUTF8(const std::wstring_view& src, yycc_u8string& dst) {
		std::string adapted_dst;
		bool ret = WcharToChar(src, adapted_dst, CP_UTF8);
		if (ret) dst = ToUTF8(adapted_dst);
		return ret;
	}
	bool WcharToUTF8(const wchar_t* src, yycc_u8string& dst) {
		CONVFCT_TYPE2(WcharToUTF8, wchar_t, yycc_char8_t);
	}
	yycc_u8string WcharToUTF8(const std::wstring_view& src) {
		CONVFCT_TYPE3(WcharToUTF8, wchar_t, yycc_char8_t);
	}
	yycc_u8string WcharToUTF8(const wchar_t* src) {
		CONVFCT_TYPE4(WcharToUTF8, wchar_t, yycc_char8_t);
	}

#pragma endregion

#pragma region UTF8ToWchar
	
	bool UTF8ToWchar(const yycc_u8string_view& src, std::wstring& dst) {
		std::string_view adapted_src(ToOrdinaryView(src));
		return CharToWchar(adapted_src, dst, CP_UTF8);
	}
	bool UTF8ToWchar(const yycc_char8_t* src, std::wstring& dst) {
		CONVFCT_TYPE2(UTF8ToWchar, yycc_char8_t, wchar_t);
	}
	std::wstring UTF8ToWchar(const yycc_u8string_view& src) {
		CONVFCT_TYPE3(UTF8ToWchar, yycc_char8_t, wchar_t);
	}
	std::wstring UTF8ToWchar(const yycc_char8_t* src) {
		CONVFCT_TYPE4(UTF8ToWchar, yycc_char8_t, wchar_t);
	}

#pragma endregion
	
#pragma region CharToUTF8

	bool CharToUTF8(const std::string_view& src, yycc_u8string& dst, UINT code_page) {
		std::string adapted_dst;
		bool ret = CharToChar(src, adapted_dst, code_page, CP_UTF8);
		if (ret) dst = ToUTF8(adapted_dst);
		return ret;
	}
	bool CharToUTF8(const char* src, yycc_u8string& dst, UINT code_page) {
		CONVFCT_TYPE2(CharToUTF8, char, yycc_char8_t, code_page);
	}
	yycc_u8string CharToUTF8(const std::string_view& src, UINT code_page) {
		CONVFCT_TYPE3(CharToUTF8, char, yycc_char8_t, code_page);
	}
	yycc_u8string CharToUTF8(const char* src, UINT code_page) {
		CONVFCT_TYPE4(CharToUTF8, char, yycc_char8_t, code_page);
	}

#pragma endregion

#pragma region UTF8ToChar

	bool UTF8ToChar(const yycc_u8string_view& src, std::string& dst, UINT code_page) {
		std::string_view adapted_src(ToOrdinaryView(src));
		return CharToChar(adapted_src, dst, CP_UTF8, code_page);
	}
	bool UTF8ToChar(const yycc_char8_t* src, std::string& dst, UINT code_page) {
		CONVFCT_TYPE2(UTF8ToChar, yycc_char8_t, char, code_page);
	}
	std::string UTF8ToChar(const yycc_u8string_view& src, UINT code_page) {
		CONVFCT_TYPE3(UTF8ToChar, yycc_char8_t, char, code_page);
	}
	std::string UTF8ToChar(const yycc_char8_t* src, UINT code_page) {
		CONVFCT_TYPE4(UTF8ToChar, yycc_char8_t, char, code_page);
	}

#pragma endregion

#endif

#undef CONVFCT_TYPE2
#undef CONVFCT_TYPE3
#undef CONVFCT_TYPE4

}

