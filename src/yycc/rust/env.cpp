#include "env.hpp"
#include "../macro/os_detector.hpp"

#if defined(YYCC_OS_WINDOWS)
#include "../encoding/windows.hpp"
#include "../num/safe_op.hpp"
#include "../num/safe_cast.hpp"
#include <Windows.h>
#include <winbase.h>
#else
#include "../string/reinterpret.hpp"
#include <cstdlib>
#include <cerrno>
#include <stdexcept>
#endif

#define SAFECAST ::yycc::num::safe_cast
#define SAFEOP ::yycc::num::safe_op
#define ENC ::yycc::encoding::windows
#define REINTERPRET ::yycc::string::reinterpret

namespace yycc::rust::env {

    EnvResult<std::u8string> get_var(const std::u8string_view &name) {
#if defined(YYCC_OS_WINDOWS)
        // Reference: https://learn.microsoft.com/en-us/windows/win32/api/processenv/nf-processenv-getenvironmentvariablew
        // Convert to wchar
        auto wname = ENC::to_wchar(name);
        if (!wname.has_value()) {
            return std::unexpected(EnvError::BadEncoding);
        }

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
            if (!fct_size.has_value()) return std::unexpected(EnvError::BadArithmetic);
            auto rv = ::GetEnvironmentVariableW(wname.value().c_str(), wvalue.data(), fct_size.value());

            // Check the return value
            if (rv == 0) {
                // Function failed. Extract error reason.
                auto ec = GetLastError();
                if (ec == ERROR_ENVVAR_NOT_FOUND) return std::unexpected(EnvError::NoSuchName);
                else return std::unexpected(EnvError::BadCall);
            } else {
                // Function okey. Check the size.
                // Fetch function expected size.
                auto rv_size = SAFECAST::try_to<size_t>(rv);
                if (!rv_size.has_value()) return std::unexpected(EnvError::BadArithmetic);
                auto exp_size = SAFEOP::checked_sub<size_t>(rv_size.value(), 1);
                if (!exp_size.has_value()) return std::unexpected(EnvError::BadArithmetic);

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
        return ENC::to_utf8(wvalue).transform_error([](auto err) { return EnvError::BadEncoding; });
#else
        // String view is not NULL-terminal-guaranted,
        // so we solve this when casting its type.
        auto ordinary_name = REINTERPRET::as_ordinary(name);

        // Fetch variable
        auto finder = std::getenv(ordinary_name.c_str());
        if (finder == nullptr) return std::unexpected(EnvError::NoSuchName);
        else return REINTERPRET::as_utf8(finder);
#endif
    }

    EnvResult<void> set_var(const std::u8string_view &name, const std::u8string_view &value) {
#if defined(YYCC_OS_WINDOWS)
        // Reference: https://learn.microsoft.com/en-us/windows/win32/api/processenv/nf-processenv-setenvironmentvariablew

        // Convert to wchar
        auto wname = ENC::to_wchar(name);
        auto wvalue = ENC::to_wchar(value);
        if (!(wname.has_value() && wvalue.has_value())) {
            return std::unexpected(EnvError::BadEncoding);
        }

        // Delete variable and check result.
        auto rv = ::SetEnvironmentVariableW(wname.value().c_str(), wvalue.value().c_str());
        if (!rv) {
            return std::unexpected(EnvError::BadCall);
        }

        return {};
#else
        // Reference: https://pubs.opengroup.org/onlinepubs/9699919799/functions/setenv.html

        // Set variable
        auto ordinary_name = REINTERPRET::as_ordinary(name);
        auto ordinary_value = REINTERPRET::as_ordinary(value);
        auto rv = setenv(ordinary_name.c_str(), ordinary_value.c_str(), 1);
        if (rv == 0) return {};

        // Check error type
        if (errno == EINVAL) return std::unexpected(EnvError::BadName);
        else if (errno == ENOMEM) return std::unexpected(EnvError::NoMemory);
        else throw std::runtime_error("impossible errno");
#endif
    }

    EnvResult<void> del_var(const std::u8string_view &name) {
#if defined(YYCC_OS_WINDOWS)
        // Reference: https://learn.microsoft.com/en-us/windows/win32/api/processenv/nf-processenv-setenvironmentvariablew

        // Convert to wchar
        auto wname = ENC::to_wchar(name);
        if (!wname.has_value()) {
            return std::unexpected(EnvError::BadEncoding);
        }

        // Delete variable and check result.
        auto rv = ::SetEnvironmentVariableW(wname.value().c_str(), NULL);
        if (!rv) {
            return std::unexpected(EnvError::BadCall);
        }

        return {};
#else
        // Reference: https://pubs.opengroup.org/onlinepubs/9699919799/functions/unsetenv.html

        // Delete variable
        auto ordinary_name = REINTERPRET::as_ordinary(name);
        auto rv = unsetenv(ordinary_name.c_str());
        if (rv == 0) return {};

        // Check error type
        if (errno == EINVAL) return std::unexpected(EnvError::BadName);
        else throw std::runtime_error("impossible errno");
#endif
    }

} // namespace yycc::rust::env
