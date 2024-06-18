#pragma once
#include "YYCCInternal.hpp"

#include <string>

#if YYCC_OS == YYCC_OS_WINDOWS
#include "WinImportPrefix.hpp"
#include <Windows.h>
#include "WinImportSuffix.hpp"
#endif

namespace YYCC::EncodingHelper {

#if YYCC_OS == YYCC_OS_WINDOWS

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

#endif

	bool UTF8ToUTF16(const char* src, std::u16string& dest);
	std::u16string UTF8ToUTF16(const char* src);
	bool UTF8ToUTF32(const char* src, std::u32string& dest);
	std::u32string UTF8ToUTF32(const char* src);

	bool UTF16ToUTF8(const char16_t* src, std::string& dest);
	std::string  UTF16ToUTF8(const char16_t* src);
	bool UTF32ToUTF8(const char32_t* src, std::string& dest);
	std::string  UTF32ToUTF8(const char32_t* src);

}
