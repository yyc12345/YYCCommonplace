#pragma once
#include "../macro/os_detector.hpp"
#include "../macro/stl_detector.hpp"
#if defined(YYCC_OS_WINDOWS)

#include <string>
#include <string_view>
#include <expected>

#include "import_guard_head.hpp"
#include <Windows.h>
#include "import_guard_tail.hpp"

namespace yycc::windows::winfct {

    /// @brief All errors occur in this module.
    enum class WinFctError {
        Win32,  ///< Error occurs when calling Win32 functions.
        NoCom,    ///< No COM environment.
    };

    /// @brief The result type used in this module.
    template<typename T>
    using WinFctResult = std::expected<T, WinFctError>;

    /**
	 * @brief Get Windows used HANDLE for current module.
	 * @details
	 * If your target is EXE, the current module simply is your program self.
	 * However, if your target is DLL, the current module is your DLL, not the EXE loading your DLL.
	 * 
	 * This function is frequently used by DLL.
	 * Because some design need the HANDLE of current module, not the host EXE loading your DLL.
	 * For example, you may want to get the path of your built DLL, or fetch resources from your DLL at runtime,
	 * then you should pass current module HANDLE, not NULL or the HANDLE of EXE.
	 * @return A Windows HANDLE pointing to current module, or error occurs.
	*/
    WinFctResult<HMODULE> get_current_module();

    /**
	 * @brief Get path to Windows temporary directory.
	 * @details Windows temporary directory usually is the target of \%TEMP\%.
	 * @return Fetched UTF8 encoded path to Windows temporary directory, or error occurs.
	*/
    WinFctResult<std::u8string> get_temp_directory();

    /**
	 * @brief Get the file name of given module HANDLE
	 * @param[in] hModule 
	 * The HANDLE to the module where you want to get file name.
	 * It is same as the HANDLE parameter of Win32 \c GetModuleFileName.
	 * @param[out] ret The variable receiving UTF8 encoded file name of given module.
	 * @return Fetched UTF8 encoded file name of given module, or error occurs.
	*/
    WinFctResult<std::u8string> get_module_file_name(HINSTANCE hModule);

    /**
	 * @brief Check whether given code page number is a valid one.
	 * @param[in] code_page The code page number.
	 * @return True if it is valid, otherwise false.
	*/
    bool is_valid_code_page(UINT code_page);

    /**
	 * @brief Copies an existing file to a new file.
	 * @param[in] lpExistingFileName The name of an existing file.
	 * @param[in] lpNewFileName The name of the new file.
	 * @param[in] bFailIfExists 
	 * If this parameter is TRUE and the new file specified by \c lpNewFileName already exists, the function fails.
	 * If this parameter is FALSE and the new file already exists, the function overwrites the existing file and succeeds.
	 * @return Nothing or error occurs. If function failed with backend error, caller can call \c GetLastError for more details.
	 * @remarks Same as Windows \c CopyFile: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-copyfilew
	*/
    WinFctResult<void> copy_file(const std::u8string_view& lpExistingFileName, const std::u8string_view& lpNewFileName, BOOL bFailIfExists);

    /**
	 * @brief Moves an existing file or a directory, including its children.
	 * @param[in] lpExistingFileName The current name of the file or directory on the local computer.
	 * @param[in] lpNewFileName 
	 * The new name for the file or directory. The new name must not already exist.
	 * A new file may be on a different file system or drive. A new directory must be on the same drive.
	 * @return Nothing or error occurs. If function failed with backend error, caller can call \c GetLastError for more details.
	 * @remarks Same as Windows \c MoveFile: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-movefilew
	*/
    WinFctResult<void> move_file(const std::u8string_view& lpExistingFileName, const std::u8string_view& lpNewFileName);

    /**
	 * @brief Deletes an existing file.
	 * @param[in] lpFileName The name of the file to be deleted.
	 * @return Nothing or error occurs. If function failed with backend error, caller can call \c GetLastError for more details.
	 * @remarks Same as Windows \c DeleteFile: https://learn.microsoft.com/e-us/windows/win32/api/winbase/nf-winbase-deletefile
	*/
    WinFctResult<void> delete_file(const std::u8string_view& lpFileName);

#if defined(YYCC_STL_MSSTL)

    /// @brief The known directory type in Windows.
    enum class KnownDirectory {
        LocalAppData, ///< The path \%LOCALAPPDATA\% pointed.
        AppData, ///< The path \%APPDATA\% pointed.
    };

    /**
     * @brief Get the path to \%LOCALAPPDATA\%.
     * @details \%LOCALAPPDATA\% usually was used as putting local app data files
     * @param[out] ret The variable receiving UTF8 encoded path to LOCALAPPDATA.
     * @return True if success, otherwise false.
    */
    WinFctResult<std::u8string> get_known_path(KnownDirectory path_type);

#endif

}

#endif
