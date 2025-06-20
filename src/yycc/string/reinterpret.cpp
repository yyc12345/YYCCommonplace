#include "reinterpret.hpp"

#define NS_YYCC_STRING ::yycc::string

namespace yycc::string::reinterpret {

	const NS_YYCC_STRING::u8char* as_utf8(const char* src) {
		return reinterpret_cast<const NS_YYCC_STRING::u8char*>(src);
	}
	NS_YYCC_STRING::u8char* as_utf8(char* src) {
		return reinterpret_cast<NS_YYCC_STRING::u8char*>(src);
	}
	NS_YYCC_STRING::u8string as_utf8(const std::string_view& src) {
		return NS_YYCC_STRING::u8string(reinterpret_cast<const NS_YYCC_STRING::u8char*>(src.data()), src.size());
	}
	NS_YYCC_STRING::u8string_view as_utf8_view(const std::string_view& src) {
		return NS_YYCC_STRING::u8string_view(reinterpret_cast<const NS_YYCC_STRING::u8char*>(src.data()), src.size());
	}

	const char* as_ordinary(const NS_YYCC_STRING::u8char* src) {
		return reinterpret_cast<const char*>(src);
	}
	char* as_ordinary(NS_YYCC_STRING::u8char* src) {
		return reinterpret_cast<char*>(src);
	}
	std::string as_ordinary(const NS_YYCC_STRING::u8string_view& src) {
		return std::string(reinterpret_cast<const char*>(src.data()), src.size());
	}
	std::string_view as_ordinary_view(const NS_YYCC_STRING::u8string_view& src) {
		return std::string_view(reinterpret_cast<const char*>(src.data()), src.size());
	}

}
