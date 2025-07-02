#pragma once
#include "YYCCInternal.hpp"

#include <string>

#if YYCC_OS == YYCC_OS_WINDOWS
#include "WinImportPrefix.hpp"
#include <Windows.h>
#include "WinImportSuffix.hpp"
#endif

/**
 * @brief The helper for all encoding stuff.
 * @details
 * For more infomations about how to use the functions provided by this namespace,
 * please see \ref library_encoding and \ref encoding_helper.
*/
namespace YYCC::EncodingHelper {

#if YYCC_OS == YYCC_OS_WINDOWS

	bool WcharToChar(const std::wstring_view& src, std::string& dst, UINT code_page);
	bool WcharToChar(const wchar_t* src, std::string& dst, UINT code_page);
	std::string WcharToChar(const std::wstring_view& src, UINT code_page);
	std::string WcharToChar(const wchar_t* src, UINT code_page);

	bool CharToWchar(const std::string_view& src, std::wstring& dst, UINT code_page);
	bool CharToWchar(const char* src, std::wstring& dst, UINT code_page);
	std::wstring CharToWchar(const std::string_view& src, UINT code_page);
	std::wstring CharToWchar(const char* src, UINT code_page);

	bool CharToChar(const std::string_view& src, std::string& dst, UINT src_code_page, UINT dst_code_page);
	bool CharToChar(const char* src, std::string& dst, UINT src_code_page, UINT dst_code_page);
	std::string CharToChar(const std::string_view& src, UINT src_code_page, UINT dst_code_page);
	std::string CharToChar(const char* src, UINT src_code_page, UINT dst_code_page);


	bool WcharToUTF8(const std::wstring_view& src, yycc_u8string& dst);
	bool WcharToUTF8(const wchar_t* src, yycc_u8string& dst);
	yycc_u8string WcharToUTF8(const std::wstring_view& src);
	yycc_u8string WcharToUTF8(const wchar_t* src);

	bool UTF8ToWchar(const yycc_u8string_view& src, std::wstring& dst);
	bool UTF8ToWchar(const yycc_char8_t* src, std::wstring& dst);
	std::wstring UTF8ToWchar(const yycc_u8string_view& src);
	std::wstring UTF8ToWchar(const yycc_char8_t* src);
	
	bool CharToUTF8(const std::string_view& src, yycc_u8string& dst, UINT code_page);
	bool CharToUTF8(const char* src, yycc_u8string& dst, UINT code_page);
	yycc_u8string CharToUTF8(const std::string_view& src, UINT code_page);
	yycc_u8string CharToUTF8(const char* src, UINT code_page);

	bool UTF8ToChar(const yycc_u8string_view& src, std::string& dst, UINT code_page);
	bool UTF8ToChar(const yycc_char8_t* src, std::string& dst, UINT code_page);
	std::string UTF8ToChar(const yycc_u8string_view& src, UINT code_page);
	std::string UTF8ToChar(const yycc_char8_t* src, UINT code_page);

#endif

}
