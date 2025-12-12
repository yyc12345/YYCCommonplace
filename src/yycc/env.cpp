#include "env.hpp"
#include "macro/os_detector.hpp"
#include <string>
#include <string_view>
#include <memory>
#include <type_traits>
#include <stdexcept>

#if defined(YYCC_OS_WINDOWS)
#include "encoding/windows.hpp"
#include "num/safe_op.hpp"
#include "num/safe_cast.hpp"
#include "windows/winfct.hpp"
#include "windows/import_guard_head.hpp"
#include <Windows.h>
#include <winbase.h>
#include <processenv.h> // For getting environment variables and commandline argument.
#include <shellapi.h>   // For getting commandline argument.
#include "windows/import_guard_tail.hpp"
#elif defined(YYCC_OS_LINUX)
#include "string/reinterpret.hpp"
#include <cstdlib>
#include <cerrno>
#include <fstream> // For reading commandline argument.
#include <unistd.h>
#include <sys/stat.h> // For reading symlink target.
#elif defined(YYCC_OS_MACOS)
#include "string/reinterpret.hpp"
#include "num/safe_cast.hpp"
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <cstring>
#include <mach-o/dyld.h> // For getting current exe path.
#include <crt_externs.h> // For getting commandline argument.
#else
#error "Not supported OS"
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

            // Parse "KEY=VALUE"
            size_t pos = entry.find(L'=');
            if (pos != std::string::npos) {
                auto key = entry.substr(0, pos);
                auto value = entry.substr(pos + 1);
                if (key.empty()) return std::unexpected(VarError::NullPointer);

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

            // Increase the pointer
            current += entry.length() + 1;
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

#if defined(YYCC_OS_LINUX)

    /// @brief All possible error occurs when reading Linux symlink target.
    enum class ReadSymlinkTargetError {
        SysCall,   ///< Fail to call system call.
        Truncated, ///< Expected path to target was truncated.
        Others,    ///< Any other errors
    };

    /**
     * @brief An utility function for convenient reading symlink target on Linux.
     * @param[in] link The path to symlink where the target is read.
     * @return The read path to to target, or error occurs.
     */
    static std::expected<std::u8string, ReadSymlinkTargetError> read_symlink_target(const std::string_view &link) {
        // Reference: https://www.man7.org/linux/man-pages/man2/readlink.2.html

        // String view is not NUL terminated.
        // Create an string container for it.
        std::string path(link);

        // Get the expected size.
        // Query this symlink info first.
        struct stat sb;
        if (lstat(path.c_str(), &sb) != 0) {
            return std::unexpected(ReadSymlinkTargetError::SysCall);
        }
        // Fetch the size of target path in gotten struct.
        // And cast it into expected type.
        auto expected_size = SAFECAST::try_to<size_t>(sb.st_size);
        if (!expected_size.has_value()) {
            return std::unexpected(ReadSymlinkTargetError::Others);
        }
        auto buf_size = expected_size.value();
        // Some magic symlinks under (for example) /proc and /sys report 'st_size' as zero.
        // In that case, take PATH_MAX as a "good enough" estimate.
        if (buf_size == 0) {
            buf_size = PATH_MAX;
        }

        // Prepare return value and allocate it with previous gotten size.
        std::u8string rv(u8'\0', buf_size);

        // Copy data into result value.
        // Add one to the link size, so that we can determine whether
        // the buffer returned by readlink() was truncated.
        // Also, due to the add operation, we need do overflow checks.
        auto passed_size = SAFEOP::checked_add<size_t>(buf_size, 1);
        if (!passed_size.has_value()) {
            return std::unexpected(ReadSymlinkTargetError::Others);
        }
        // Read data into result value.
        ssize_t nbytes = readlink(path.c_str(), REINTERPRET::as_ordinary(rv.data()), passed_size.value());
        if (nbytes < 0) {
            return std::unexpected(ReadSymlinkTargetError::SysCall);
        }

        // Check written size
        // Cast it type into expected type.
        auto written_size = SAFECAST::try_to<size_t>(nbytes);
        if (!written_size.has_value()) {
            return std::unexpected(ReadSymlinkTargetError::Others);
        }
        // If the return value was equal to the buffer size, then
        // the link target was larger than expected (perhaps because the
        // target was changed between the call to lstat() and the call to
        // readlink()). Return error instead.
        if (written_size.value() != buf_size) {
            // TODO: There must be a better solution to this truncated issue than simply return error.
            return std::unexpected(ReadSymlinkTargetError::Truncated);
        }

        // Everything is okey
        return rv;
    }

#endif

    PathResult<std::filesystem::path> current_exe() {
        std::u8string rv;

#if defined(YYCC_OS_WINDOWS)
        auto file_name = WINFCT::get_module_file_name(NULL);
        if (file_name.has_value()) rv = std::move(file_name.value());
        else return std::unexpected(PathError::SysCall);
#elif defined(YYCC_OS_LINUX)
        // Reference: https://www.man7.org/linux/man-pages/man5/proc_pid_exe.5.html
        auto target = read_symlink_target("/proc/self/exe");
        if () return rv = std::move(target.value());
        else return std::unexpected(PathError::SysCall);
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
        void operator()(LPWSTR* ptr) {
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
        for (int i = 1; i < argc; ++i) { // starts with 1 to remove first part (executable self)
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
                // Check whether reading is okey.
                if (!cmdline.good()) return std::unexpected(ArgError::Others);
                // If return string is empty, it means that we reach the tail.
                if (arg.empty()) break;

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
