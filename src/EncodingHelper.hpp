#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include <string>

#include "WinImportPrefix.hpp"
#include <Windows.h>
#include "WinImportSuffix.hpp"

namespace YYCC::EncodingHelper {

	bool WcharToChar(const wchar_t* src, u8string& dest, UINT codepage);
	bool WcharToUTF8(const wchar_t* src, u8string& dest);
	u8string WcharToChar(const wchar_t* src, UINT codepage);
	u8string WcharToUTF8(const wchar_t* src);

	bool CharToWchar(const u8char* src, std::wstring& dest, UINT codepage);
	bool UTF8ToWchar(const u8char* src, std::wstring& dest);
	std::wstring CharToWchar(const u8char* src, UINT codepage);
	std::wstring UTF8ToWchar(const u8char* src);

	bool CharToChar(const u8char* src, u8string& dest, UINT src_codepage, UINT dest_codepage);
	u8string CharToChar(const u8char* src, UINT src_codepage, UINT dest_codepage);

}

#endif