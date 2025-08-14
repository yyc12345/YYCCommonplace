#pragma once
#include <string>
#include <string_view>
#include <expected>

/**
 * @brief The namespace providing environment variable operations.
 * @details
 * When I programming with Rust, I was astonished that
 * Rust standard library have so much robust environment variable operations.
 * Oppositly, C++ STL still lake in this even in today.
 *
 * The functions manipulating environment variable is different in different OS.
 * I create this namespace inspired from Rust standard library
 * to glue all these things up and make a uniform interface.
 */
namespace yycc::rust::env {

    /// @brief The error occurs in this module.
    enum class EnvError {
        NoSuchName,    ///< The variable with given name is not presented.
        BadEncoding,   ///< Error when performing encoding convertion.
        BadArithmetic, ///< Error when performing arithmetic operations.
        BadCall,       ///< Error occurs when calling backend functions.
        BadName,       ///< Given name is ill-formated (empty string or has "=" character).
        NoMemory,      ///< No enough memory to finish this operation.
    };

    /// @brief The result type in this module.
    template<typename T>
    using EnvResult = std::expected<T, EnvError>;

    /**
     * @brief Get the value of given environment variable name.
     * @param[in] name The name of environment variable
     * @return Gotten value, or error occurs.
     */
    EnvResult<std::u8string> get_var(const std::u8string_view& name);

    /**
     * @brief Set the value of given environment variable name.
     * @details
     * If there is no such name variable presented in environment,
     * a new variable will be created,
     * otherwise, new value will overwrite old value.
     * @param[in] name The name of environment variable
     * @param[in] value The value to be written into.
     * @return Nothing or error occurs.
     */
    EnvResult<void> set_var(const std::u8string_view& name, const std::u8string_view& value);

    /**
     * @brief Delete environment variable with given name.
     * @details
     * If given variable is not presented in environment,
     * this function will NOT return error.
     * @param[in] name The name of environment variable
     * @return Nothing, or error occurs.
     */
    EnvResult<void> del_var(const std::u8string_view& name);

} // namespace yycc::rust::env
