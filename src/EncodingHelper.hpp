#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include <string>

#include "WinImportPrefix.hpp"
#include <Windows.h>
#include "WinImportSuffix.hpp"

namespace YYCC::EncodingHelper {

	bool WcharToChar(const wchar_t* src, std::string& dest, UINT codepage);
	bool WcharToChar(const std::wstring& src, std::string& dest, UINT codepage);
	std::string WcharToChar(const wchar_t* src, UINT codepage);
	std::string WcharToChar(const std::wstring& src, UINT codepage);

	bool CharToWchar(const char* src, std::wstring& dest, UINT codepage);
	bool CharToWchar(const std::string& src, std::wstring& dest, UINT codepage);
	std::wstring CharToWchar(const char* src, UINT codepage);
	std::wstring CharToWchar(const std::string& src, UINT codepage);

	bool CharToChar(const char* src, std::string& dest, UINT src_codepage, UINT dest_codepage);
	bool CharToChar(const std::string& src, std::string& dest, UINT src_codepage, UINT dest_codepage);
	std::string CharToChar(const char* src, UINT src_codepage, UINT dest_codepage);
	std::string CharToChar(const std::string& src, UINT src_codepage, UINT dest_codepage);

}

#endif