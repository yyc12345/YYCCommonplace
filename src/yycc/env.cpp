#include "env.hpp"
#include "macro/os_detector.hpp"

// Environment variable required
#if defined(YYCC_OS_WINDOWS)
#include "encoding/windows.hpp"
#include "num/safe_op.hpp"
#include "num/safe_cast.hpp"
#include <Windows.h>
#include <winbase.h>
#else
#include "string/reinterpret.hpp"
#include <cstdlib>
#include <cerrno>
#include <stdexcept>
#endif

// Path related functions required
#if defined(YYCC_OS_WINDOWS)
#include "windows/winfct.hpp"
#else
#include <unistd.h>
#include <sys/stat.h>
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

    std::vector<VarPair> get_vars() {
        // TODO: finish this function according to Rust implementation.
        // Considering whether replace return value with an iterator.
        throw std::logic_error("not implemented");
    }

#pragma endregion

#pragma region Environment Path

    PathResult<std::filesystem::path> current_dir() {
        return std::filesystem::current_path();
    }

    PathResult<std::filesystem::path> current_exe() {
#if defined(YYCC_OS_WINDOWS)
        return WINFCT::get_module_file_name(NULL).transform_error([](auto e) { return PathError::SysCall; }).transform([](auto v) {
            return std::filesystem::path(v);
        });
#elif defined(YYCC_OS_LINUX)
        // Reference: https://www.man7.org/linux/man-pages/man2/readlink.2.html

        // specify the path
        constexpr char path[] = "/proc/self/exe";

        // get the expected size
        struct stat sb;
        if (lstat(path, &sb) != 0) {
        }
        auto expected_size = SAFECAST::try_to<size_t>(sb.st_size);
        if (!expected_size.has_value()) {
            return std::unexpected(PathError::Others);
        }
        auto buf_size = expected_size.value();
        // Some magic symlinks under (for example) /proc and /sys report 'st_size' as zero.
        // In that case, take PATH_MAX as a "good enough" estimate.
        if (buf_size == 0) {
            buf_size = PATH_MAX;
        }

        // prepare buffer and resize it;
        std::u8string rv(u8'\0', buf_size);

        // write data
        auto passed_size = SAFEOP::checked_add<size_t>(buf_size, 1);
        if (!passed_size.has_value()) {
            return std::unexpected(PathError::Others);
        }
        ssize_t nbytes = readlink(path, REINTERPRET::as_ordinary(rv.data()), passed_size.value());
        if (nbytes < 0) {
            return std::unexpected(PathError::Others);
        }

        // check written size
        auto written_size = SAFECAST::try_to<size_t>(nbytes);
        if (!written_size.has_value()) {
            return std::unexpected(PathError::Others);
        }
        if (written_size.value() != buf_size) {
            return std::unexpected(PathError::Others);
        }

        // okey
        return std::filesystem::path(rv);
#else
        // TODO: Implement this in other OS.
        // "/proc/self/exe" is Linux specific, not in POSIX standard.
        // This method may need further patch when running on macOS.
#endif
    }

    PathResult<std::filesystem::path> home_dir() {
        // TODO: finish this function according to Rust implementation.
        throw std::logic_error("not implemented");
    }

    PathResult<std::filesystem::path> temp_dir() {
        return std::filesystem::temp_directory_path();
    }

#pragma endregion

#pragma region Environment Argument

    ArgResult<std::vector<std::u8string>> get_args() {
        // TODO: finish this function according to Rust implementation.
        // Considering whether use iterator as return value.
        throw std::logic_error("not implemented");
    }

#pragma endregion

} // namespace yycc::env
