#include "EncodingHelper.hpp"

#include <locale>

namespace YYCC::EncodingHelper {

#if YYCC_OS == YYCC_OS_WINDOWS

	bool WcharToChar(const wchar_t* src, std::string& dest, UINT codepage) {
		int count, write_result;

		//converter to CHAR
		count = WideCharToMultiByte(codepage, 0, reinterpret_cast<LPCWCH>(src), -1, NULL, 0, NULL, NULL);
		if (count <= 0) return false;

		dest.resize(count - 1);
		write_result = WideCharToMultiByte(codepage, 0, reinterpret_cast<LPCWCH>(src), -1, reinterpret_cast<LPSTR>(dest.data()), count, NULL, NULL);
		if (write_result <= 0) return false;

		return true;
	}
	bool WcharToUTF8(const wchar_t* src, std::string& dest) {
		return WcharToChar(src, dest, CP_UTF8);
	}
	std::string WcharToChar(const wchar_t* src, UINT codepage) {
		std::string ret;
		if (!WcharToChar(src, ret, codepage)) ret.clear();
		return ret;
	}
	std::string WcharToUTF8(const wchar_t* src) {
		return WcharToChar(src, CP_UTF8);
	}

	bool CharToWchar(const char* src, std::wstring& dest, UINT codepage) {
		int wcount, write_result;

		// convert to WCHAR
		wcount = MultiByteToWideChar(codepage, 0, reinterpret_cast<LPCCH>(src), -1, NULL, 0);
		if (wcount <= 0) return false;

		dest.resize(wcount - 1);
		write_result = MultiByteToWideChar(codepage, 0, reinterpret_cast<LPCCH>(src), -1, reinterpret_cast<LPWSTR>(dest.data()), wcount);
		if (write_result <= 0) return false;

		return true;
	}
	bool UTF8ToWchar(const char* src, std::wstring& dest) {
		return CharToWchar(src, dest, CP_UTF8);
	}
	std::wstring CharToWchar(const char* src, UINT codepage) {
		std::wstring ret;
		if (!CharToWchar(src, ret, codepage)) ret.clear();
		return ret;
	}
	std::wstring UTF8ToWchar(const char* src) {
		return CharToWchar(src, CP_UTF8);
	}

	bool CharToChar(const char* src, std::string& dest, UINT src_codepage, UINT dest_codepage) {
		std::wstring intermediary;
		if (!CharToWchar(src, intermediary, src_codepage)) return false;
		if (!WcharToChar(intermediary.c_str(), dest, dest_codepage)) return false;
		return true;
	}
	std::string CharToChar(const char* src, UINT src_codepage, UINT dest_codepage) {
		std::string ret;
		if (!CharToChar(src, ret, src_codepage, dest_codepage)) ret.clear();
		return ret;
	}

#endif

#if defined(__cpp_char8_t)
	using CodecvtUTF8Char_t = char8_t;
#else
	using CodecvtUTF8Char_t = char;
#endif
	template<typename _TChar, std::enable_if_t<std::is_same_v<_TChar, char16_t> || std::is_same_v<_TChar, char32_t>, int> = 0>
	using CodecvtFacet_t = std::codecvt<_TChar, CodecvtUTF8Char_t, std::mbstate_t>;

	template<typename _TChar, std::enable_if_t<std::is_same_v<_TChar, char16_t> || std::is_same_v<_TChar, char32_t>, int> = 0>
	static bool UTF8ToUTFOther(const char* _src, std::basic_string<_TChar>& dest) {
		// Reference:
		// https://zh.cppreference.com/w/cpp/locale/codecvt/in
		
		// init src string
		if (_src == nullptr) return false;
		std::string src(_src);

		// init locale and get codecvt facet
		// same reason in UTFOtherToUTF8 to keeping reference to locale
		const auto& this_locale = std::locale::classic();
		const auto& this_codecvt = std::use_facet<CodecvtFacet_t<_TChar>>(this_locale);
		
		// convertion preparation
		std::mbstate_t mb{};
		dest.resize(src.size());
		const CodecvtUTF8Char_t* intern_from = reinterpret_cast<const CodecvtUTF8Char_t*>(src.c_str()),
			*intern_from_end = reinterpret_cast<const CodecvtUTF8Char_t*>(src.c_str() + src.size()),
			*intern_from_next = nullptr;
		_TChar* extern_to = dest.data(),
			*extern_to_end = dest.data() + dest.size(),
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
		dest.resize(extern_to_next - dest.data());
		return true;
	}

	bool UTF8ToUTF16(const char* src, std::u16string& dest) {
		return UTF8ToUTFOther<char16_t>(src, dest);
	}
	std::u16string UTF8ToUTF16(const char* src) {
		std::u16string ret;
		if (!UTF8ToUTF16(src, ret)) ret.clear();
		return ret;
	}
	bool UTF8ToUTF32(const char* src, std::u32string& dest) {
		return UTF8ToUTFOther<char32_t>(src, dest);
	}
	std::u32string UTF8ToUTF32(const char* src) {
		std::u32string ret;
		if (!UTF8ToUTF32(src, ret)) ret.clear();
		return ret;
	}

	template<typename _TChar, std::enable_if_t<std::is_same_v<_TChar, char16_t> || std::is_same_v<_TChar, char32_t>, int> = 0>
	static bool UTFOtherToUTF8(const _TChar* _src, std::string& dest) {
		// Reference:
		// https://zh.cppreference.com/w/cpp/locale/codecvt/out
		
		// initialize src string
		if (_src == nullptr) return false;
		std::basic_string<_TChar> src(_src);

		// init locale and get codecvt facet
		// the reference to locale must be preserved until convertion done.
		// because the life time of codecvt facet is equal to the reference to locale.
		const auto& this_locale = std::locale::classic();
		const auto& this_codecvt = std::use_facet<CodecvtFacet_t<_TChar>>(this_locale);

		// do convertion preparation
		std::mbstate_t mb{};
		dest.resize(src.size() * this_codecvt.max_length());
		const _TChar* intern_from = src.c_str(),
			*intern_from_end = src.c_str() + src.size(),
			*intern_from_next = nullptr;
		CodecvtUTF8Char_t* extern_to = reinterpret_cast<CodecvtUTF8Char_t*>(dest.data()),
			*extern_to_end = reinterpret_cast<CodecvtUTF8Char_t*>(dest.data() + dest.size()),
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
		dest.resize(extern_to_next - reinterpret_cast<CodecvtUTF8Char_t*>(dest.data()));
		return true;
	}

	bool UTF16ToUTF8(const char16_t* src, std::string& dest) {
		return UTFOtherToUTF8<char16_t>(src, dest);
	}
	std::string UTF16ToUTF8(const char16_t* src) {
		std::string ret;
		if (!UTF16ToUTF8(src, ret)) ret.clear();
		return ret;
	}
	bool UTF32ToUTF8(const char32_t* src, std::string& dest) {
		return UTFOtherToUTF8<char32_t>(src, dest);
	}
	std::string UTF32ToUTF8(const char32_t* src) {
		std::string ret;
		if (!UTF32ToUTF8(src, ret)) ret.clear();
		return ret;
	}

}

