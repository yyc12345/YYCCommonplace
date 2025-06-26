#pragma once
#include "../string/stringify.hpp"

namespace yycc::rust::stringify {

    // There is no modification for legacy "stringify" functions like "parse".
    // So we simply expose all functions into this namespace.
    using namespace ::yycc::string::stringify;

} // namespace yycc::rust::stringify
