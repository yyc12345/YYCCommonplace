#pragma once
#include <cstdint>
#include <cstddef>
#include <string_view>

/**
 * @brief The namespace providing primitive types.
 * @details
 * When I writing with Rust, I notice that most pf primitive types has explicit and exact size, and their names are short and clear.
 * Hoeever, in C++, most primitive types are variable based on system, due to the legacy of C era.
 * All primitive types with explicit size are only can be fetched in a specific header file,
 * and, their names are too long because they can not be registered as reserved keywords or names
 * due to the name conflict with so much code written in past years.
 * However, STL can't do this but I can do this.
 * I invent this namespace providing primitive types, such as integers, floating-point numbers, and strings, 
 * in Rust style, their names areshort and clear.
 */
namespace yycc::primitive {

	// `bool` is keyword so should not declare it anymore.
	// `char` is keyword so should not declare it anymore.

	using i8 = std::int8_t;
	using i16 = std::int16_t;
	using i32 = std::int32_t;
	using i64 = std::int64_t;
	using u8 = std::uint8_t;
	using u16 = std::uint16_t;
	using u32 = std::uint32_t;
	using u64 = std::uint64_t;

	using isize = std::ptrdiff_t;
	using usize = std::size_t;

	using f32 = float;
	using f64 = double;

	// using String = std::u8string;
	// using str = std::u8string_view;
}
