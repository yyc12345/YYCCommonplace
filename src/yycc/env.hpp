#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <filesystem>
#include <expected>

/**
 * @brief The namespace providing runtime environment operations.
 * @details
 * When I programming with Rust, I was astonished that
 * Rust standard library have so much robust environment-related operations,
 * such as environment variable operations, current program infos and etc.
 * Oppositly, C++ STL are still lack in this even in today.
 * So I create this namespace to glue all these things up, 
 * according to different operating systems, and make a uniform interface.
 */
namespace yycc::env {

#pragma region Environment Variable

    /// @brief The error occurs in environment variable operations.
    enum class VarError {
        SysCall,       ///< Error occurs when calling backend functions.
        NoSuchName,    ///< The variable with given name is not presented.
        BadName,       ///< Given name is ill-formated (empty string or has "=" character).
        BadEncoding,   ///< Error when performing encoding convertion.
        NoMemory,      ///< No enough memory to finish this operation.
        Others,        ///< Any other error types.
    };

    /// @brief The result type in environment variable operations.
    template<typename T>
    using VarResult = std::expected<T, VarError>;

    /**
     * @brief The pair representing an environment variable.
     * @details The left side is the name of variable. The right side is the content of variable.
     */
    using VarPair = std::pair<std::u8string, std::u8string>;

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

    /**
     * @brief Returns an list of (variable, value) pairs of strings, 
     * for all the environment variables of the current process.
     * @return The list holding all variables.
     */
    std::vector<VarPair> get_vars();

#pragma endregion

#pragma region Environment Path

    /// @brief Error occurs when operating path related functions.
    enum class PathError {
        SysCall, ///< Underlying system calling error.
        Others,  ///< Any other error types.
    };

    /// @brief The result type used for path related functions;
    template<typename T>
    using PathResult = std::expected<T, PathError>;

    /**
     * @brief Returns the current working directory.
     * @return Current working directory path or error occurs.
     */
    PathResult<std::filesystem::path> current_dir();

    /**
     * @brief Returns the path of the current running executable.
     * @return Current running executable path or error occurs.
     * Please note that there is no guarantee that return path is absolute path.
     */
    PathResult<std::filesystem::path> current_exe();

    /**
     * @brief Returns the path of the current user's home directory if known.
     * @return Current user's home directory path or error occurs.
     */
    PathResult<std::filesystem::path> home_dir();

    /**
     * @brief Returns the path of a temporary directory.
     * @return The path of a temporary directory.
     */
    PathResult<std::filesystem::path> temp_dir();

#pragma endregion

#pragma region Environment Argument

    /// @brief Error occurs when operating argument related functions.
    enum class ArgError {
        SysCall, ///< Underlying system calling error.
        Others,  ///< Any other error types.
    };

    /// @brief The result type used for argument related functions;
    template<typename T>
    using ArgResult = std::expected<T, ArgError>;

    /**
     * @brief Returns the arguments that this program was started with (normally passed via the command line).
     * @return The list holding all argument one by one.
     */
    std::vector<std::u8string> get_args();

#pragma endregion

} // namespace yycc::env
