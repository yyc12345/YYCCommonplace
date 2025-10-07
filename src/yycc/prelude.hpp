#pragma once

// Rust prelude section
#include "primitive.hpp"
#include "result.hpp"
#include "option.hpp"
#include "panic.hpp"
#include <vector>

namespace yycc::prelude {
    // Include primitive types

#define NS_YYCC_PRIMITIVE ::yycc::primitive

    using i8 = NS_YYCC_PRIMITIVE::i8;
    using i16 = NS_YYCC_PRIMITIVE::i16;
    using i32 = NS_YYCC_PRIMITIVE::i32;
    using i64 = NS_YYCC_PRIMITIVE::i64;
    using u8 = NS_YYCC_PRIMITIVE::u8;
    using u16 = NS_YYCC_PRIMITIVE::u16;
    using u32 = NS_YYCC_PRIMITIVE::u32;
    using u64 = NS_YYCC_PRIMITIVE::u64;

    using isize = NS_YYCC_PRIMITIVE::isize;
    using usize = NS_YYCC_PRIMITIVE::usize;

    using f32 = NS_YYCC_PRIMITIVE::f32;
    using f64 = NS_YYCC_PRIMITIVE::f64;

    using str = NS_YYCC_PRIMITIVE::str;

#undef NS_YYCC_PRIMITIVE

    // Other types
    using String = std::u8string;
    template<typename T>
    using Vec = std::vector<T>;

    // Expose Result and Option
    using namespace ::yycc::option;
    using namespace ::yycc::result;

    // Panic are introduced by including header file
    // so we do not need re-expose it.

} // namespace yycc::prelude

// Expose all members
using namespace ::yycc::prelude;
