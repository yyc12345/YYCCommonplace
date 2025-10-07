#pragma once
/**
 * @file
 * @brief The Rust-like prelude header for C++.
 * @details
 * When I writting with Rust, I notice Rust add types for all files in default.
 * This default imported types are called "prelude".
 * This is very convenient for programming so I decide to introduce it in C++.
 * 
 * I create this file, organize all types, which I think should be exposed for programmer, in to an independent namespace,
 * and expose them into global namesoace.
 * By simply include this file at the top of your C++ code, you can get Rust-like prelude effect in C++.
 * These exposed types including primitive types, string types, basic Option and Result utilities, panic mechanisim and etc.
*/

// Rust prelude section
#include "primitive.hpp"
#include "result.hpp"
#include "option.hpp"
#include "panic.hpp"
#include <vector>

/**
 * @brief The namespace including all types presented in prelude.
 * @details
 * By including this file, all of these types are automaticalling exposed to global namespace.
 * There is no need to refer this namespace anymore.
 * This namespace is just a container for types which need to be exposed.
 */
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

#undef NS_YYCC_PRIMITIVE

    // Other types
    //using str = std::u8string_view;
    //using String = std::u8string;
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
