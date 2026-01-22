#include "env.hpp"
#include "macro/os_detector.hpp"
#include "num/safe_op.hpp"
#include "num/safe_cast.hpp"

#if defined(YYCC_OS_WINDOWS)
// Windows headers
#include "encoding/windows.hpp"
#include "windows/winfct.hpp"
#include <memory>           // For safely free gotten environment variable and commandline argument.
#include <type_traits>      // Used by std::unique_ptr to get underlying pointer type.
#include "windows/import_guard_head.hpp"
#include <Windows.h>
#include <winbase.h>
#include <processenv.h>     // For getting environment variables and commandline argument.
#include <shellapi.h>       // For getting commandline argument.
#include "windows/import_guard_tail.hpp"
#else
// POSIX headers
#include "string/reinterpret.hpp"
#include <cstdlib>          // For POSIX environment variable operations.
#include <cerrno>           // For POSIX errno.
#include <stdexcept>        // For re-throw unexpected POSIX errno as STL exception.
#if defined(YYCC_OS_LINUX)
// Linux-only headers
#include <fstream>          // For reading commandline argument.
#elif defined(YYCC_OS_MACOS)
// macOS-only headers
#include <mach-o/dyld.h>    // For getting current exe path.
#include <crt_externs.h>    // For getting commandline argument.
#else
#error "Not supported OS"
#endif
#endif

#define SAFECAST ::yycc::num::safe_cast
#define SAFEOP ::yycc::num::safe_op
#define ENC ::yycc::encoding::windows
#define REINTERPRET ::yycc::string::reinterpret
#define WINFCT ::yycc::windows::winfct

namespace yycc::env {

#pragma region Environment Variable

    VarResult<std::u8string> get_var(const std::u8string_view &name) {
#if defined(YYCC_OS_WINDOWS)
        // Reference: https://learn.microsoft.com/en-us/windows/win32/api/processenv/nf-processenv-getenvironmentvariablew
        // Convert to wchar
        auto wname = ENC::to_wchar(name).value();

        // Prepare a variable with proper init size for holding value.
        std::wstring wvalue;
        wvalue.resize(256);

        // Start to fetch value
        while (true) {
            // YYC MARK:
            // Due to the shitty design of GetEnvironmentVariableW,
            // the size passed to this function must include NULL terminal.
            // So we forcely use checked add and sub for this bad behavior.
            auto fct_size = SAFEOP::checked_add<size_t>(wvalue.size(), 1);
            if (!fct_size.has_value()) return std::unexpected(VarError::Others);
            auto rv = ::GetEnvironmentVariableW(wname.c_str(), wvalue.data(), fct_size.value());

            // Check the return value
            if (rv == 0) {
                // Function failed. Extract error reason.
                auto ec = GetLastError();
                if (ec == ERROR_ENVVAR_NOT_FOUND) return std::unexpected(VarError::NoSuchName);
                else return std::unexpected(VarError::SysCall);
            } else {
                // Function okey. Check the size.
                // Fetch function expected size.
                auto rv_size = SAFECAST::try_to<size_t>(rv);
                if (!rv_size.has_value()) return std::unexpected(VarError::Others);
                auto exp_size = SAFEOP::checked_sub<size_t>(rv_size.value(), 1);
                if (!exp_size.has_value()) return std::unexpected(VarError::Others);

                // YYC MARK:
                // According to Microsoft, the return value of this function is just a bullshit.
                // If "wvalue" is big enough, the range of return value is [0, wvalue.size()],
                // indicating the size of final string, excluding NULL terminal.
                // otherwise, the range of return value is [wvalue.size() + 1, +inf),
                // indicating the required size of buffer including NULL terminal.
                // So we must treat this return value carefully.
                if (exp_size.value() <= wvalue.size()) {
                    // Okey, shrink to the real size of value and break.
                    wvalue.resize(rv_size.value());
                    break;
                } else {
                    // We need resize it and try again.
                    wvalue.resize(exp_size.value());
                    continue;
                }
            }
        }

        // Convert back to UTF8 string and return.
        return ENC::to_utf8(wvalue).transform_error([](auto err) { return VarError::BadEncoding; });
#else
        // String view is not NULL-terminal-guaranted,
        // so we solve this when casting its type.
        auto ordinary_name = REINTERPRET::as_ordinary(name);

        // Fetch variable
        auto finder = std::getenv(ordinary_name.c_str());
        if (finder == nullptr) return std::unexpected(VarError::NoSuchName);
        else return REINTERPRET::as_utf8(finder);
#endif
    }

    VarResult<void> set_var(const std::u8string_view &name, const std::u8string_view &value) {
#if defined(YYCC_OS_WINDOWS)
        // Reference: https://learn.microsoft.com/en-us/windows/win32/api/processenv/nf-processenv-setenvironmentvariablew

        // Convert to wchar, set variable, and check result.
        auto rv = ::SetEnvironmentVariableW(ENC::to_wchar(name).value().c_str(), ENC::to_wchar(value).value().c_str());
        if (!rv) return std::unexpected(VarError::SysCall);
        else return {};
#else
        // Reference: https://pubs.opengroup.org/onlinepubs/9699919799/functions/setenv.html

        // Set variable
        auto ordinary_name = REINTERPRET::as_ordinary(name);
        auto ordinary_value = REINTERPRET::as_ordinary(value);
        auto rv = setenv(ordinary_name.c_str(), ordinary_value.c_str(), 1);
        if (rv == 0) return {};

        // Check error type
        if (errno == EINVAL) return std::unexpected(VarError::BadName);
        else if (errno == ENOMEM) return std::unexpected(VarError::NoMemory);
        else throw std::runtime_error("impossible errno");
#endif
    }

    VarResult<void> del_var(const std::u8string_view &name) {
#if defined(YYCC_OS_WINDOWS)
        // Reference: https://learn.microsoft.com/en-us/windows/win32/api/processenv/nf-processenv-setenvironmentvariablew

        // Convert to wchar, delete variable, and check result.
        auto rv = ::SetEnvironmentVariableW(ENC::to_wchar(name).value().c_str(), NULL);
        if (!rv) return std::unexpected(VarError::SysCall);
        else return {};
#else
        // Reference: https://pubs.opengroup.org/onlinepubs/9699919799/functions/unsetenv.html

        // Delete variable
        auto ordinary_name = REINTERPRET::as_ordinary(name);
        auto rv = unsetenv(ordinary_name.c_str());
        if (rv == 0) return {};

        // Check error type
        if (errno == EINVAL) return std::unexpected(VarError::BadName);
        else throw std::runtime_error("impossible errno");
#endif
    }

#if defined(YYCC_OS_WINDOWS)

    class EnvironmentStringsDeleter {
    public:
        EnvironmentStringsDeleter() {}
        void operator()(LPWCH ptr) {
            if (ptr != nullptr) {
                FreeEnvironmentStringsW(ptr);
            }
        }
    };
    using SmartEnvironmentStrings = std::unique_ptr<std::remove_pointer_t<LPWCH>, EnvironmentStringsDeleter>;

#else

    extern char** environ;

#endif

    VarResult<std::vector<VarPair>> get_vars() {
        // TODO: Considering whether replace return value with an iterator.
        std::vector<VarPair> rv;

#if defined(YYCC_OS_WINDOWS)
        // Reference: https://learn.microsoft.com/en-us/windows/win32/api/processenv/nf-processenv-getenvironmentstringsw

        SmartEnvironmentStrings env_block(GetEnvironmentStringsW());
        if (env_block == nullptr) return std::unexpected(VarError::SysCall);

        wchar_t *current = env_block.get();
        while (*current != L'\0') {
            // Fetch current wide string
            std::wstring_view entry(current);
            // Increase the pointer first,
            // because in later steps we may enter next loop before reaching the end of this loop syntax.
            current += entry.length() + 1;

            // Parse "KEY=VALUE"
            size_t pos = entry.find(L'=');
            if (pos != std::string::npos) {
                auto key = entry.substr(0, pos);
                auto value = entry.substr(pos + 1);
                // However, Idk why there are some shit words like "=::=::" are in this string list.
                // This should be excluded and can not be seen as error.
                // So we simply skip them and do not report error.
                if (key.empty()) continue;

                auto u8key = ENC::to_utf8(key);
                auto u8value = ENC::to_utf8(value);
                if (u8key.has_value() && u8value.has_value()) {
                    rv.emplace_back(std::make_pair(std::move(u8key.value()), std::move(u8value.value())));
                } else {
                    return std::unexpected(VarError::BadEncoding);
                }
            } else {
                return std::unexpected(VarError::Others);
            }
        }

        env_block.reset();
#else
        // Reference: https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap08.html

        // POSIX (Linux, macOS, etc.)
        for (char **env = environ; *env != nullptr; ++env) {
            std::string_view entry(*env);
            size_t pos = entry.find('=');
            if (pos != std::string::npos) {
                auto key = entry.substr(0, pos);
                auto value = entry.substr(pos + 1);
                if (key.empty()) return std::unexpected(VarError::NullPointer);
                rv.emplace_back(std::make_pair(REINTERPRET::as_utf8(key), REINTERPRET::as_utf8(value)));
            } else {
                return std::unexpected(VarError::Others);
            }
        }
#endif

        return rv;
    }

#pragma endregion

#pragma region Environment Path

    PathResult<std::filesystem::path> current_dir() {
        return std::filesystem::current_path();
    }

    PathResult<std::filesystem::path> current_exe() {
        std::u8string rv;

#if defined(YYCC_OS_WINDOWS)
        auto file_name = WINFCT::get_module_file_name(NULL);
        if (file_name.has_value()) rv = std::move(file_name.value());
        else return std::unexpected(PathError::SysCall);
#elif defined(YYCC_OS_LINUX)
        // Reference: https://www.man7.org/linux/man-pages/man5/proc_pid_exe.5.html
        std::error_code ec;
        auto target = std::filesystem::read_symlink(std::filesystem::path("/proc/self/exe"), ec);
        if (ec) return std::unexpected(PathError::SysCall);
        else rv = REINTERPRET::as_utf8(target.string());
#elif defined(YYCC_OS_MACOS)
        // TODO: This is AI generated and don't have test and reference.
        std::string buffer(PATH_MAX, '\0');
        auto rv_size = SAFECAST::try_to<uint32_t>(buffer.size());
        if (!rv_size.has_value()) return std::unexpected(PathError::Others);
        auto size = rv_size.value();
        if (_NSGetExecutablePath(buffer.data(), &size) != 0) {
            // Buffer too small; resize and retry
            buffer.resize(SAFECAST::to<size_t>(size));
            if (_NSGetExecutablePath(buffer.data(), &size) != 0) {
                return std::unexpected(PathError::SysCall);
            }
        }
        buffer.resize(strlen(buffer.data()));
        rv = REINTERPRET::as_utf8(buffer);
#else
#error "Not supported OS"
#endif

        return std::filesystem::path(rv);
    }

    PathResult<std::filesystem::path> home_dir() {
        std::u8string rv;

#if defined(YYCC_OS_WINDOWS)
        // USERPROFILE is introduced in Rust std::env::home_dir() document.
        auto home = get_var(u8"USERPROFILE");
        if (home.has_value()) rv = std::move(home.value());
        else return std::unexpected(PathError::SysCall);
#else
        // Reference: https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap08.html

        // HOME is an environment variable in POSIX standard.
        auto home = get_var(u8"HOME");
        if (home.has_value()) rv = std::move(home.value());
        else return std::unexpected(PathError::SysCall);
#endif

        return std::filesystem::path(rv);
    }

    PathResult<std::filesystem::path> temp_dir() {
        return std::filesystem::temp_directory_path();
    }

#pragma endregion

#pragma region Environment Argument

#if defined(YYCC_OS_WINDOWS)

    class CommandLineArgvDeleter {
    public:
        CommandLineArgvDeleter() {}
        void operator()(LPWSTR *ptr) {
            if (ptr != nullptr) {
                LocalFree(ptr);
            }
        }
    };
    using SmartCommandLineArgv = std::unique_ptr<std::remove_pointer_t<LPWSTR *>, CommandLineArgvDeleter>;

#endif

    ArgResult<std::vector<std::u8string>> get_args() {
        // TODO: Considering whether use iterator as return value.
        std::vector<std::u8string> rv;

#if defined(YYCC_OS_WINDOWS)
        // Reference: https://learn.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-commandlinetoargvw

        // Fetch args from Win32 functions
        int argc;
        SmartCommandLineArgv argv(CommandLineToArgvW(GetCommandLineW(), &argc));
        if (argv == nullptr) return std::unexpected(ArgError::NullPointer);

        // Analyse it
        for (int i = 0; i < argc; ++i) { // starts with 1 to remove first part (executable self)
            auto arg = argv.get()[i];
            if (arg == nullptr) return std::unexpected(ArgError::NullPointer);

            auto u8arg = ENC::to_utf8(arg);
            if (u8arg.has_value()) {
                rv.emplace_back(std::move(u8arg.value()));
            } else {
                return std::unexpected(ArgError::BadEncoding);
            }
        }

        // Free data
        argv.reset();
#elif defined(YYCC_OS_LINUX)
        // Reference: https://www.man7.org/linux/man-pages/man5/proc_pid_cmdline.5.html

        // Open special file.
        // Because we are in Linux so we do not need use UTF-8.
        std::ifstream cmdline("/proc/self/cmdline", std::ios::binary);
        // Check whether file is open.
        if (cmdline.is_open()) {
            // Prepare container for holding fetched argument.
            std::string arg;
            // Fetch arguments one by one.
            while (true) {
                // We use NUL as delimiter
                std::getline(cmdline, arg, '\0');
                // Check whether we reach EOF
                if (cmdline.eof()) break;
                // Check whether reading is okey.
                if (cmdline.fail()) return std::unexpected(ArgError::Others);

                // Push this argument into result.
                rv.emplace_back(REINTERPRET::as_utf8(arg));
            }
            // Close file
            cmdline.close();
        } else {
            return std::unexpected(ArgError::Others);
        }

#elif defined(YYCC_OS_MACOS)
        // TODO: This is AI generated and don't have test and reference.
        char ***apple_argv = _NSGetArgv();
        int *apple_argc = _NSGetArgc();
        if (apple_argv && apple_argc) {
            for (int i = 0; i < *apple_argc; ++i) {
                auto ptr = (*apple_argv)[i];
                if (ptr == nullptr) return std::unexpected(ArgError::NullPointer);
                else rv.emplace_back(REINTERPRET::as_utf8(ptr));
            }
        } else {
            return std::unexpected(ArgError::SysCall);
        }
#else
#error "Not supported OS"
#endif

        return rv;
    }

#pragma endregion

} // namespace yycc::env
