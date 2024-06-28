#pragma once
#include "YYCCInternal.hpp"

#include <string>

#if YYCC_OS == YYCC_OS_WINDOWS
#include "WinImportPrefix.hpp"
#include <Windows.h>
#include "WinImportSuffix.hpp"
#endif

/**
 * @brief The namespace handling encoding issues.
 * @details
 * \par Windows Encoding Convertion
 * This namespace provides the convertion between wchar_t, UTF8 and code-page-based string:
 * The function name has following format: \c AAAToBBB.
 * AAA is the source string and BBB is target string.
 * AAA and BBB has following possible value:
 * \li \c Char: Code-page-based string. Usually it will add a code page parameter for function to get the code page of this string. For code page, please see Microsoft document.
 * \li \c UTF8: UTF8 string.
 * \li \c Wchar: wchar_t string.
 * \par
 * For example: \c WcharToUTF8 will perform the convertion from wchar_t to UTF8,
 * and \c CharToChar will perform the convertion between 2 code-page-based string and caller can specify individual code page for these 2 string.
 * \par
 * These functions are Windows specific and are unavailable on other platforms.
 * Becasue Windows use wchar_t string as its function arguments for globalization, and this library use UTF8 everywhere.
 * So it should have a bidirectional way to do convertion between wchar_t string and UTF8 string.
 *
 * \par UTF32, UTF16 and UTF8 Convertion
 * This namespace also provide the convertion among UTF32, UTF16 and UTF8.
 * These convertion functions are suit for all platforms, not Windows oriented.
 * \par
 * Due to implementation, this library assume all non-Windows system use UTF8 as their C locale.
 * Otherwise these functions will produce wrong result.
 *
 * \par Function Parameters
 * We provide these encoding convertion functions with following 2 types:
 * \li Function returns \c bool and its parameter order source string pointer and a corresponding \c std::basic_string container for receiving result.
 * \li Function returns corresponding \c std::basic_string result, and its parameter only order source string pointer.
 * \par
 * For these 2 declarations, both of them will not throw any exception and do not accept nullptr as source string.
 * The only difference is that the way to indicate convertion error.
 * \par
 * First declaration will return false to indicate there is an error when doing convertion. Please note that the content of string container passing in may still be changed!
 * Last declaration will return empty string to indicate error. Please note if you pass empty string in, they still will output empty string but it doesn't mean an error.
 * So last declaration is used in the scenario that we don't care whether the convertion success did. For example, output something to console.
 *
*/
namespace YYCC::EncodingHelper {

#define _YYCC_U8(strl) u8 ## strl
#define YYCC_U8(strl) (reinterpret_cast<const ::YYCC::yycc_char8_t*>(_YYCC_U8(strl)))

	const yycc_char8_t* ToUTF8(const char* src);
	yycc_char8_t* ToUTF8(char* src);
	yycc_u8string ToUTF8(const std::string_view& src);
	yycc_u8string_view ToUTF8View(const std::string_view& src);

	const char* ToNative(const yycc_char8_t* src);
	char* ToNative(yycc_char8_t* src);
	std::string ToNative(const yycc_u8string_view& src);
	std::string_view ToNativeView(const yycc_u8string_view& src);

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
