#pragma once

// Define the UTF8 char type we used.
// And do a polyfill if no embedded char8_t type.

#include "macro/feature_probe.hpp"
#include <string>
#include <string_view>

namespace yycc::string {
    /**
		\typedef u8char_t
		\brief YYCC UTF8 char type.
		\details
		This char type is an alias to \c char8_t if your current C++ standard support it.
		Otherwise it is defined as <TT>unsigned char</TT> as C++ 20 stdandard does.
	*/
    /**
		\typedef u8string
		\brief YYCC UTF8 string container type.
		\details
		This type is defined as \c std::basic_string<yycc_char8_t>.
		It is equal to \c std::u8string if your current C++ standard support it.
	*/
    /**
		\typedef u8string_view
		\brief YYCC UTF8 string view type.
		\details
		This type is defined as \c std::basic_string_view<yycc_char8_t>.
		It is equal to \c std::u8string_view if your current C++ standard support it.
	*/

#if defined(YYCC_CPPFEAT_UTF8)
    using u8char = char8_t;
    using u8string = std::u8string;
    using u8string_view = std::u8string_view;
#else
    using u8char = unsigned char;
    using u8string = std::basic_string<u8char>;
    using u8string_view = std::basic_string_view<u8char>;
#endif

#define _YYCC_U8(strl) u8 ## strl ///< The assistant macro for YYCC_U8.
#define YYCC_U8(strl) (reinterpret_cast<const ::yycc::string::u8char*>(_YYCC_U8(strl))) ///< The macro for creating UTF8 string literal. See \ref library_encoding.
#define YYCC_U8_CHAR(chr) (static_cast<::yycc::string::u8char>(chr)) ///< The macro for casting ordinary char type into YYCC UTF8 char type.

} // namespace yycc::string
