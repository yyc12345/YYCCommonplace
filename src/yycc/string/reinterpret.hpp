#pragma once
#include "../string.hpp"

#define NS_YYCC_STRING ::yycc::string

namespace yycc::string::reinterpret {

#define _YYCC_U8(strl) u8 ## strl ///< The assistant macro for YYCC_U8.
#define YYCC_U8(strl) (reinterpret_cast<const ::yycc::string::u8char*>(_YYCC_U8(strl))) ///< The macro for creating UTF8 string literal. See \ref library_encoding.
#define YYCC_U8_CHAR(chr) (static_cast<::yycc::string::u8char>(chr)) ///< The macro for casting ordinary char type into YYCC UTF8 char type.

	const NS_YYCC_STRING::u8char* as_utf8(const char* src);
	NS_YYCC_STRING::u8char* as_utf8(char* src);
	NS_YYCC_STRING::u8string as_utf8(const std::string_view& src);
	NS_YYCC_STRING::u8string_view as_utf8_view(const std::string_view& src);

	const char* as_ordinary(const NS_YYCC_STRING::u8char* src);
	char* as_ordinary(NS_YYCC_STRING::u8char* src);
	std::string as_ordinary(const NS_YYCC_STRING::u8string_view& src);
	std::string_view as_ordinary_view(const NS_YYCC_STRING::u8string_view& src);

}

#undef NS_YYCC_STRING
