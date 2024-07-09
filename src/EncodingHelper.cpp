#include "EncodingHelper.hpp"

#include <locale>

namespace YYCC::EncodingHelper {

#pragma region UTF8 Ordinary Convertion
	
	const yycc_char8_t* ToUTF8(const char* src) {
		return reinterpret_cast<const yycc_char8_t*>(src);
	}
	yycc_char8_t* ToUTF8(char* src) {
		return reinterpret_cast<yycc_char8_t*>(src);
	}
	yycc_u8string ToUTF8(const std::string_view& src) {
		return yycc_u8string(reinterpret_cast<const yycc_char8_t*>(src.data()), src.size());
	}
	yycc_u8string_view ToUTF8View(const std::string_view& src) {
		return yycc_u8string_view(reinterpret_cast<const yycc_char8_t*>(src.data()), src.size());
	}
	
	const char* ToOrdinary(const yycc_char8_t* src) {
		return reinterpret_cast<const char*>(src);
	}
	char* ToOrdinary(yycc_char8_t* src) {
		return reinterpret_cast<char*>(src);
	}
	std::string ToOrdinary(const yycc_u8string_view& src) {
		return std::string(reinterpret_cast<const char*>(src.data()), src.size());
	}
	std::string_view ToOrdinaryView(const yycc_u8string_view& src) {
		return std::string_view(reinterpret_cast<const char*>(src.data()), src.size());
	}

#pragma endregion

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

	
#pragma region UTF8 UTF16 UTF32 Help Funcs

	/*
	According to the documentation introduced in CppReference.
	The standard library is guaranteed to provide several specific specializations of \c std::codecvt.
	The UTF8 char type in UTF8 related specializations of \c std::codecvt is different.
	It is also independend from we defined \c yycc_char8_t.
	So it is essential define a type which can correctly trigger specific specializations of \c std::codecv in there.
	*/
#if defined(__cpp_char8_t)
	using CodecvtUTF8Char_t = char8_t;
#else
	using CodecvtUTF8Char_t = char;
#endif

	template<typename _TChar, std::enable_if_t<std::is_same_v<_TChar, char16_t> || std::is_same_v<_TChar, char32_t>, int> = 0>
	using CodecvtFacet_t = std::codecvt<_TChar, CodecvtUTF8Char_t, std::mbstate_t>;

	template<typename _TChar, std::enable_if_t<std::is_same_v<_TChar, char16_t> || std::is_same_v<_TChar, char32_t>, int> = 0>
	static bool UTF8ToUTFOther(const yycc_u8string_view& src, std::basic_string<_TChar>& dst) {
		// Reference:
		// https://zh.cppreference.com/w/cpp/locale/codecvt/in
		
		// if src is empty, return directly
		if (src.empty()) {
			dst.clear();
			return true;
		}

		// init locale and get codecvt facet
		// same reason in UTFOtherToUTF8 to keeping reference to locale
		const auto& this_locale = std::locale::classic();
		const auto& this_codecvt = std::use_facet<CodecvtFacet_t<_TChar>>(this_locale);
		
		// convertion preparation
		std::mbstate_t mb{};
		dst.resize(src.size());
		const CodecvtUTF8Char_t* intern_from = reinterpret_cast<const CodecvtUTF8Char_t*>(src.data()),
			*intern_from_end = reinterpret_cast<const CodecvtUTF8Char_t*>(src.data() + src.size()),
			*intern_from_next = nullptr;
		_TChar* extern_to = dst.data(),
			*extern_to_end = dst.data() + dst.size(),
			*extern_to_next = nullptr;
		// do convertion
		auto result = this_codecvt.in(
			mb,
			intern_from, intern_from_end, intern_from_next,
			extern_to, extern_to_end, extern_to_next
		);

		// check result
		if (result != CodecvtFacet_t<_TChar>::ok)
			return false;
		// resize result and return
		dst.resize(extern_to_next - dst.data());
		return true;
	}

	template<typename _TChar, std::enable_if_t<std::is_same_v<_TChar, char16_t> || std::is_same_v<_TChar, char32_t>, int> = 0>
	static bool UTFOtherToUTF8(const std::basic_string_view<_TChar>& src, yycc_u8string& dst) {
		// Reference:
		// https://zh.cppreference.com/w/cpp/locale/codecvt/out
		
		// if src is empty, return directly
		if (src.empty()) {
			dst.clear();
			return true;
		}

		// init locale and get codecvt facet
		// the reference to locale must be preserved until convertion done.
		// because the life time of codecvt facet is equal to the reference to locale.
		const auto& this_locale = std::locale::classic();
		const auto& this_codecvt = std::use_facet<CodecvtFacet_t<_TChar>>(this_locale);

		// do convertion preparation
		std::mbstate_t mb{};
		dst.resize(src.size() * this_codecvt.max_length());
		const _TChar* intern_from = src.data(),
			*intern_from_end = src.data() + src.size(),
			*intern_from_next = nullptr;
		CodecvtUTF8Char_t* extern_to = reinterpret_cast<CodecvtUTF8Char_t*>(dst.data()),
			*extern_to_end = reinterpret_cast<CodecvtUTF8Char_t*>(dst.data() + dst.size()),
			*extern_to_next = nullptr;
		// do convertion
		auto result = this_codecvt.out(
			mb,
			intern_from, intern_from_end, intern_from_next,
			extern_to, extern_to_end, extern_to_next
		);

		// check result
		if (result != CodecvtFacet_t<_TChar>::ok)
			return false;
		// resize result and retuen
		dst.resize(extern_to_next - reinterpret_cast<CodecvtUTF8Char_t*>(dst.data()));
		return true;
	}

#pragma endregion

#pragma region UTF8ToUTF16
	
	bool UTF8ToUTF16(const yycc_u8string_view& src, std::u16string& dst) {
		return UTF8ToUTFOther<char16_t>(src, dst);
	}
	bool UTF8ToUTF16(const yycc_char8_t* src, std::u16string& dst) {
		CONVFCT_TYPE2(UTF8ToUTF16, yycc_char8_t, char16_t);
	}
	std::u16string UTF8ToUTF16(const yycc_u8string_view& src) {
		CONVFCT_TYPE3(UTF8ToUTF16, yycc_char8_t, char16_t);
	}
	std::u16string UTF8ToUTF16(const yycc_char8_t* src) {
		CONVFCT_TYPE4(UTF8ToUTF16, yycc_char8_t, char16_t);
	}

#pragma endregion

#pragma region UTF16ToUTF8
	
	bool UTF16ToUTF8(const std::u16string_view& src, yycc_u8string& dst) {
		return UTFOtherToUTF8<char16_t>(src, dst);
	}
	bool UTF16ToUTF8(const char16_t* src, yycc_u8string& dst) {
		CONVFCT_TYPE2(UTF16ToUTF8, char16_t, yycc_char8_t);
	}
	yycc_u8string UTF16ToUTF8(const std::u16string_view& src) {
		CONVFCT_TYPE3(UTF16ToUTF8, char16_t, yycc_char8_t);
	}
	yycc_u8string UTF16ToUTF8(const char16_t* src) {
		CONVFCT_TYPE4(UTF16ToUTF8, char16_t, yycc_char8_t);
	}

#pragma endregion
	
#pragma region UTF8ToUTF32
	
	bool UTF8ToUTF32(const yycc_u8string_view& src, std::u32string& dst) {
		return UTF8ToUTFOther<char32_t>(src, dst);
	}
	bool UTF8ToUTF32(const yycc_char8_t* src, std::u32string& dst) {
		CONVFCT_TYPE2(UTF8ToUTF32, yycc_char8_t, char32_t);
	}
	std::u32string UTF8ToUTF32(const yycc_u8string_view& src) {
		CONVFCT_TYPE3(UTF8ToUTF32, yycc_char8_t, char32_t);
	}
	std::u32string UTF8ToUTF32(const yycc_char8_t* src) {
		CONVFCT_TYPE4(UTF8ToUTF32, yycc_char8_t, char32_t);
	}

#pragma endregion

#pragma region UTF32ToUTF8
	
	bool UTF32ToUTF8(const std::u32string_view& src, yycc_u8string& dst) {
		return UTFOtherToUTF8<char32_t>(src, dst);
	}
	bool UTF32ToUTF8(const char32_t* src, yycc_u8string& dst) {
		CONVFCT_TYPE2(UTF32ToUTF8, char32_t, yycc_char8_t);
	}
	yycc_u8string UTF32ToUTF8(const std::u32string_view& src) {
		CONVFCT_TYPE3(UTF32ToUTF8, char32_t, yycc_char8_t);
	}
	yycc_u8string UTF32ToUTF8(const char32_t* src) {
		CONVFCT_TYPE4(UTF32ToUTF8, char32_t, yycc_char8_t);
	}

#pragma endregion

#undef CONVFCT_TYPE2
#undef CONVFCT_TYPE3
#undef CONVFCT_TYPE4

}

