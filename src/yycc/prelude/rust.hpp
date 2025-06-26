#pragma once

// Include YYCC prelude first
#include "core.hpp"

// Rust prelude section
#include "../rust/primitive.hpp"
#include "../rust/result.hpp"
#include "../rust/option.hpp"
#include "../rust/panic.hpp"
#include <vector>

namespace yycc::prelude::rust {
    // Include primitive types

#define NS_RUST_PRIMITIVE ::yycc::rust::primitive

    using i8 = NS_RUST_PRIMITIVE::i8;
    using i16 = NS_RUST_PRIMITIVE::i16;
    using i32 = NS_RUST_PRIMITIVE::i32;
    using i64 = NS_RUST_PRIMITIVE::i64;
    using u8 = NS_RUST_PRIMITIVE::u8;
    using u16 = NS_RUST_PRIMITIVE::u16;
    using u32 = NS_RUST_PRIMITIVE::u32;
    using u64 = NS_RUST_PRIMITIVE::u64;

    using isize = NS_RUST_PRIMITIVE::isize;
    using usize = NS_RUST_PRIMITIVE::usize;

    using f32 = NS_RUST_PRIMITIVE::f32;
    using f64 = NS_RUST_PRIMITIVE::f64;

    using str = NS_RUST_PRIMITIVE::str;

#undef NS_RUST_PRIMITIVE

    // Other types
    using String = ::yycc::string::u8string;
    template<typename T>
    using Vec = std::vector<T>;

    // Expose Result and Option
    using namespace ::yycc::rust::option;
    using namespace ::yycc::rust::result;

    // Panic are introduced by including header file
    // so we do not need re-expose it.

} // namespace yycc::prelude::rust

// Expose all members
using namespace yycc::prelude::rust;
