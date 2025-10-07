#pragma once
#include "patch/format.hpp"
#include <string_view>
#include <format>

/**
 * @brief Provides Rust-style panic functionality for immediate program termination on unrecoverable errors.
 * @details
 * This namespace provides macros and functions to handle unrecoverable errors in C++ code.
 * It imitate Rust's \c panic! macro behavior, allowing the program to immediately exit with error information and stack traces.
 *
 * After writing programs in Rust, I deeply realized the necessity of handling errors immediately.
 * When encountering unrecoverable errors, the program should exit immediately and report the error, which ensures program robustness.
 * Therefore, I introduced this namespace and implemented macros and functions equivalent to Rust's \c panic! macro.
 * 
 * Unfortunately, I cannot change the exception mechanism in the standard library.
 * The standard library will still throw exceptions where it does, and I cannot prevent that.
 * Therefore, I suggest a good practice that any C++ exception should be immediately treated as an error and cause the program to crash and exit.
 * For this reason, registering any unhandled error callbacks which may resume the execution of program is prohibited to prevent unexpected continuation of execution.
 * For code we write ourselves that we can control, we should use the macros provided in this file instead of throwing exceptions.
 * In this way, unexpected behavior in our code will cause the program to exit immediately, outputting error information and stack traces.
 * Standard library exceptions will also cause the program to exit, but without stack information.
 */
namespace yycc::panic {

    /**
     * @brief Immediately crashes the entire program like Rust's \c panic! macro.
     * @details
     * The macro parameters are the message to format and its arguments, following \c std::format syntax.
     * This macro essentially calls \c std::format internally.
     * However, this format function is specially modified that it can accept UTF8 format string and UTF8 string argument.
     * More preciously, it is "format" in \c yycc::patch::format namespace.
     */
#define RS_PANIC(msg, ...) ::yycc::panic::panic(__FILE__, __LINE__, ::yycc::patch::format::format(msg __VA_OPT__(, ) __VA_ARGS__))

    /**
     * @brief Immediately crashes the entire program like Rust's \c panic! macro.
     * @details
     * This is the actual crash output function called by the macros.
     * The crash information will be written to \c stderr, including stack traces if your C++ runtime support it.
     * @param[in] file Source file name where panic occurred. Usually filled by macros.
     * @param[in] line Line number in source file where panic occurred. Usually filled by macros.
     * @param[in] msg Message to display during panic.
     */
    [[noreturn]] void panic(const char* file, int line, const std::u8string_view& msg);

} // namespace yycc::panic
