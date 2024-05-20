#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include <string>

#include "WinImportPrefix.hpp"
#include <Windows.h>
#include "WinImportSuffix.hpp"

namespace YYCC::EncodingHelper {

	bool WcharToChar(const wchar_t* src, std::string& dest, UINT codepage);
	bool WcharToUTF8(const wchar_t* src, std::string& dest);
	std::string WcharToChar(const wchar_t* src, UINT codepage);
	std::string WcharToUTF8(const wchar_t* src);

	bool CharToWchar(const char* src, std::wstring& dest, UINT codepage);
	bool UTF8ToWchar(const char* src, std::wstring& dest);
	std::wstring CharToWchar(const char* src, UINT codepage);
	std::wstring UTF8ToWchar(const char* src);

	bool CharToChar(const char* src, std::string& dest, UINT src_codepage, UINT dest_codepage);
	std::string CharToChar(const char* src, UINT src_codepage, UINT dest_codepage);

}

#endif