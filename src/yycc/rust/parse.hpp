#pragma once
#include "../macro/feature_probe.hpp"
#include "../string/parse.hpp"
#include "panic.hpp"
#include "result.hpp"

#define NS_YYCC_STRING_PARSE ::yycc::string::parse

namespace yycc::rust::parse {

#if defined(YYCC_CPPFEAT_EXPECTED)

    using Error = NS_YYCC_STRING_PARSE::ParseError;

    // template<typename T>
    // using Result = std::expected<T, Error>;



#endif

}

#undef NS_YYCC_STRING_PARSE
