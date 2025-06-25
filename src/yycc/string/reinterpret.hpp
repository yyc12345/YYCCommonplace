#pragma once
#include "../string.hpp"

#define NS_YYCC_STRING ::yycc::string

/**
 * @brief Provides utilities for reinterpretation between UTF-8 and ordinary string types.
 * @details
 * Please note that there is no encoding convertion happended in this namespace provided functions.
 * They just simply reinterpret one string to another string.
 * The validation of UTF8 string is guaranteed by user self.
 */
namespace yycc::string::reinterpret {

#define _YYCC_U8(strl) u8 ## strl ///< The assistant macro for YYCC_U8.
#define YYCC_U8(strl) (reinterpret_cast<const ::yycc::string::u8char*>(_YYCC_U8(strl))) ///< The macro for creating UTF8 string literal. See \ref library_encoding.
#define YYCC_U8_CHAR(chr) (static_cast<::yycc::string::u8char>(chr)) ///< The macro for casting ordinary char type into YYCC UTF8 char type.

	/**
	 * @brief Reinterpret ordinary C-string to UTF-8 string (const version).
	 * @param src Source ordinary string
	 * @return Pointer to UTF-8 encoded string
	 */
	const NS_YYCC_STRING::u8char* as_utf8(const char* src);
	/**
	 * @brief Reinterpret ordinary C-string as an UTF-8 string (non-const version).
	 * @param src Source ordinary string
	 * @return Pointer to UTF-8 encoded string
	 */
	NS_YYCC_STRING::u8char* as_utf8(char* src);
	/**
	 * @brief Reinterpret ordinary string view to copied UTF-8 string.
	 * @param src Source ordinary string view
	 * @return UTF-8 encoded string
	 */
	NS_YYCC_STRING::u8string as_utf8(const std::string_view& src);
	/**
	 * @brief Reinterpret ordinary string view to UTF-8 string view.
	 * @param src Source ordinary string view
	 * @return UTF-8 encoded string view
	 */
	NS_YYCC_STRING::u8string_view as_utf8_view(const std::string_view& src);

	/**
	 * @brief Reinterpret UTF-8 C-string to ordinary string (const version).
	 * @param src Source UTF-8 string
	 * @return Pointer to ordinary string
	 */
	const char* as_ordinary(const NS_YYCC_STRING::u8char* src);
	/**
	 * @brief Reinterpret UTF-8 C-string to ordinary string (non-const version).
	 * @param src Source UTF-8 string
	 * @return Pointer to ordinary string
	 */
	char* as_ordinary(NS_YYCC_STRING::u8char* src);
	/**
	 * @brief Reinterpret UTF-8 string view to ordinary string.
	 * @param src Source UTF-8 string view
	 * @return Ordinary string
	 */
	std::string as_ordinary(const NS_YYCC_STRING::u8string_view& src);
	/**
	 * @brief Reinterpret UTF-8 string view to ordinary string view
	 * @param src Source UTF-8 string view
	 * @return Ordinary string view
	 */
	std::string_view as_ordinary_view(const NS_YYCC_STRING::u8string_view& src);

}

#undef NS_YYCC_STRING
