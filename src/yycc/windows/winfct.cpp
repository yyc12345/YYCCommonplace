#include "winfct.hpp"
#if defined(YYCC_OS_WINDOWS)

#include "../encoding/windows.hpp"
#include <stdexcept>

#if defined(YYCC_STL_MSSTL)
#include "com.hpp"
#endif

#define ENC ::yycc::encoding::windows
#define COM ::yycc::windows::com

namespace yycc::windows::winfct {

    WinFctResult<HMODULE> get_current_module() {
        // Reference: https://stackoverflow.com/questions/557081/how-do-i-get-the-hmodule-for-the-currently-executing-code
        HMODULE hModule = NULL;
        BOOL rv = ::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
                                           | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, // get address but do not inc ref counter.
                                       (LPCWSTR) get_current_module,
                                       &hModule);
        if (rv) return hModule;
        else return std::unexpected(WinFctError::Win32);
    }

    WinFctResult<std::u8string> get_temp_directory() {
        // create wchar buffer for receiving the temp path.
        std::wstring wpath(MAX_PATH + 1u, L'\0');
        DWORD expected_size;

        // fetch temp folder
        while (true) {
            if ((expected_size = ::GetTempPathW(static_cast<DWORD>(wpath.size()), wpath.data())) == 0) {
                // failed, return
                return std::unexpected(WinFctError::Win32);
            }

            if (expected_size > static_cast<DWORD>(wpath.size())) {
                // buffer is too short, need enlarge and do fetching again
                wpath.resize(expected_size);
            } else {
                // ok. shrink to real length. break while
                wpath.resize(expected_size);
                break;
            }
        }

        // convert to utf8 and return
        return ENC::to_utf8(wpath).value();
    }

    WinFctResult<std::u8string> get_module_file_name(HINSTANCE hModule) {
        // create wchar buffer for receiving the temp path.
        std::wstring wpath(MAX_PATH + 1u, L'\0');
        DWORD copied_size;

        while (true) {
            if ((copied_size = ::GetModuleFileNameW(hModule, wpath.data(), static_cast<DWORD>(wpath.size()))) == 0) {
                // failed, return
                return std::unexpected(WinFctError::Win32);
            }

            // check insufficient buffer
            if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
                // buffer is not enough, enlarge it and try again.
                wpath.resize(wpath.size() + MAX_PATH);
            } else {
                // ok. shrink to real length. break while
                wpath.resize(copied_size);
                break;
            }
        }

        // convert to utf8 and return
        return ENC::to_utf8(wpath).value();
    }

    bool is_valid_code_page(UINT code_page) {
        CPINFOEXW cpinfo;
        return ::GetCPInfoExW(code_page, 0, &cpinfo);
    }

    WinFctResult<void> copy_file(const std::u8string_view& lpExistingFileName, const std::u8string_view& lpNewFileName, BOOL bFailIfExists) {
        auto wExistingFileName = ENC::to_wchar(lpExistingFileName).value();
        auto wNewFileName = ENC::to_wchar(lpNewFileName).value();

        if (!::CopyFileW(wExistingFileName.c_str(), wNewFileName.c_str(), bFailIfExists)) {
            return std::unexpected(WinFctError::Win32);
        }

        return {};
    }

    WinFctResult<void> move_file(const std::u8string_view& lpExistingFileName, const std::u8string_view& lpNewFileName) {
        auto wExistingFileName = ENC::to_wchar(lpExistingFileName).value();
        auto wNewFileName = ENC::to_wchar(lpNewFileName).value();

        if (!::MoveFileW(wExistingFileName.c_str(), wNewFileName.c_str())) {
            return std::unexpected(WinFctError::Win32);
        }

        return {};
    }

    WinFctResult<void> delete_file(const std::u8string_view& lpFileName) {
        auto wFileName = ENC::to_wchar(lpFileName).value();

        if (!::DeleteFileW(wFileName.c_str())) {
            return std::unexpected(WinFctError::Win32);
        }

        return {};
    }

#if defined(YYCC_STL_MSSTL)

    WinFctResult<std::u8string> get_known_path(KnownDirectory path_type) {
        // check whether com initialized
        if (!COM::is_initialized()) return std::unexpected(WinFctError::NoCom);

        // get folder id according to type
        const KNOWNFOLDERID* pId;
        switch (path_type) {
            case KnownDirectory::LocalAppData:
                pId = &FOLDERID_LocalAppData;
                break;
            case KnownDirectory::AppData:
                pId = &FOLDERID_RoamingAppData;
                break;
            default:
                throw std::logic_error("unknow known directory type");
        }

        // fetch path
        LPWSTR raw_known_path;
        HRESULT hr = SHGetKnownFolderPath(*pId, KF_FLAG_CREATE, NULL, &raw_known_path);
        if (FAILED(hr)) return std::unexpected(WinFctError::Win32);
        COM::SmartLPWSTR known_path(raw_known_path);

        // convert to utf8 and return
        return ENC::to_utf8(known_path.get()).value();
    }

#endif

} // namespace yycc::windows::winfct

#endif
