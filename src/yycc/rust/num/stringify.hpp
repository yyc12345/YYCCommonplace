#pragma once
#include "../../num/stringify.hpp"

namespace yycc::rust::num::stringify {

    // There is no modification for legacy "stringify" functions like "parse".
    // So we simply expose all functions into this namespace.
    using namespace ::yycc::num::stringify;

} // namespace yycc::rust::stringify
