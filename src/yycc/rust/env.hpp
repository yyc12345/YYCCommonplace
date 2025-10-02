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

#pragma region Environment Variable

    /// @brief The error occurs in environment variable operations.
    enum class VarError {
        NoSuchName,    ///< The variable with given name is not presented.
        BadEncoding,   ///< Error when performing encoding convertion.
        BadArithmetic, ///< Error when performing arithmetic operations.
        BadCall,       ///< Error occurs when calling backend functions.
        BadName,       ///< Given name is ill-formated (empty string or has "=" character).
        NoMemory,      ///< No enough memory to finish this operation.
    };

    /// @brief The result type in environment variable operations.
    template<typename T>
    using VarResult = std::expected<T, VarError>;

    /**
     * @brief Get the value of given environment variable name.
     * @param[in] name The name of environment variable
     * @return Gotten value, or error occurs.
     */
    VarResult<std::u8string> get_var(const std::u8string_view& name);

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
    VarResult<void> set_var(const std::u8string_view& name, const std::u8string_view& value);

    /**
     * @brief Delete environment variable with given name.
     * @details
     * If given variable is not presented in environment,
     * this function will NOT return error.
     * @param[in] name The name of environment variable
     * @return Nothing, or error occurs.
     */
    VarResult<void> del_var(const std::u8string_view& name);

#pragma endregion

#pragma region Environment Path

    /// @brief Error occurs when operating path related functions.
    enum class PathError {
        Win32,    ///< Underlying Win32 function error.
        Posix,    ///< Underlying POSIX failed.
        BadSize,  ///< Written size if not matched with expected size.
        BadCast,  ///< Error occurs when casting values.
        Overflow, ///< Some arithmetic operation overflow.
    };

    /// @brief The result type used for path related functions;
    template<typename T>
    using PathResult = std::expected<T, PathError>;

    /**
     * @brief Get the path of the current running executable.
     * @return Gotten path (no absolute path guaranteed) or error occurs.
     */
    PathResult<std::u8string> current_exe();

#pragma endregion

} // namespace yycc::rust::env
