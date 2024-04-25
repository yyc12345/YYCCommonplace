#pragma once
#include <string>

#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS
#include "WinImportPrefix.hpp"
#include <Windows.h>
#include <fileapi.h>
#include "WinImportSuffix.hpp"
#endif

namespace YYCC::EncodingHelper {
#if YYCC_OS == YYCC_OS_WINDOWS

	bool WcharToChar(const wchar_t* src, std::string& dest, const UINT codepage);
	bool WcharToChar(const std::wstring& src, std::string& dest, const UINT codepage);

	bool CharToWchar(const char* src, std::wstring& dest, const UINT codepage);
	bool CharToWchar(const std::string& src, std::wstring& dest, const UINT codepage);

	bool CharToChar(const char* src, std::string& dest, const UINT src_codepage, const UINT dest_codepage);
	bool CharToChar(const std::string& src, std::string& dest, const UINT src_codepage, const UINT dest_codepage);

#endif
}
