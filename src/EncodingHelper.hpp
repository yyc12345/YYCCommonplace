#pragma once
#include "YYCCInternal.hpp"

#include <string>

#if YYCC_OS == YYCC_OS_WINDOWS
#include "WinImportPrefix.hpp"
#include <Windows.h>
#include "WinImportSuffix.hpp"
#endif

/**
 * @brief The helper for all encoding aspects.
 * @details
 * For more infomations about how to use the functions provided by this namespace,
 * please see \ref library_encoding and \ref encoding_helper.
*/
namespace YYCC::EncodingHelper {

#define _YYCC_U8(strl) u8 ## strl ///< The assistant macro for YYCC_U8.
#define YYCC_U8(strl) (reinterpret_cast<const ::YYCC::yycc_char8_t*>(_YYCC_U8(strl))) ///< The macro for creating UTF8 string literal. See \ref library_encoding.

	const yycc_char8_t* ToUTF8(const char* src);
	yycc_char8_t* ToUTF8(char* src);
	yycc_u8string ToUTF8(const std::string_view& src);
	yycc_u8string_view ToUTF8View(const std::string_view& src);

	const char* ToOrdinary(const yycc_char8_t* src);
	char* ToOrdinary(yycc_char8_t* src);
	std::string ToOrdinary(const yycc_u8string_view& src);
	std::string_view ToOrdinaryView(const yycc_u8string_view& src);

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

#endif

	bool UTF8ToUTF16(const yycc_u8string_view& src, std::u16string& dst);
	bool UTF8ToUTF16(const yycc_char8_t* src, std::u16string& dst);
	std::u16string UTF8ToUTF16(const yycc_u8string_view& src);
	std::u16string UTF8ToUTF16(const yycc_char8_t* src);

	bool UTF16ToUTF8(const std::u16string_view& src, yycc_u8string& dst);
	bool UTF16ToUTF8(const char16_t* src, yycc_u8string& dst);
	yycc_u8string UTF16ToUTF8(const std::u16string_view& src);
	yycc_u8string UTF16ToUTF8(const char16_t* src);


	bool UTF8ToUTF32(const yycc_u8string_view& src, std::u32string& dst);
	bool UTF8ToUTF32(const yycc_char8_t* src, std::u32string& dst);
	std::u32string UTF8ToUTF32(const yycc_u8string_view& src);
	std::u32string UTF8ToUTF32(const yycc_char8_t* src);

	bool UTF32ToUTF8(const std::u32string_view& src, yycc_u8string& dst);
	bool UTF32ToUTF8(const char32_t* src, yycc_u8string& dst);
	yycc_u8string UTF32ToUTF8(const std::u32string_view& src);
	yycc_u8string UTF32ToUTF8(const char32_t* src);

}
