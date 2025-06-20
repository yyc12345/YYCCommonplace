#pragma once

// Prelude section
#include "../string.hpp"
namespace yycc::prelude {

#define NS_YYCC_STRING ::yycc::string

    using u8char = NS_YYCC_STRING::u8char;
    using u8string = NS_YYCC_STRING::u8string;
    using u8string_view = NS_YYCC_STRING::u8string_view;

#undef NS_YYCC_STRING

} // namespace yycc::prelude

// Expose all members
using namespace yycc::prelude;
