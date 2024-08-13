#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include <string>

#include "WinImportPrefix.hpp"
#include <Windows.h>
#include "WinImportSuffix.hpp"

/**
 * @brief The helper providing assistance of Win32 functions.
 * @details
 * This helper is Windows specific.
 * If current environment is not Windows, the whole namespace will be unavailable.
 * See also \ref win_fct_helper
*/
namespace YYCC::WinFctHelper {
	
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
	 * @return A Windows HANDLE pointing to current module, NULL if failed.
	*/
	HMODULE GetCurrentModule();

	/**
	 * @brief Get path to Windows temporary folder.
	 * @details Windows temporary folder usually is the target of \%TEMP\%.
	 * @param[out] ret The variable receiving UTF8 encoded path to Windows temp folder.
	 * @return True if success, otherwise false.
	*/
	bool GetTempDirectory(yycc_u8string& ret);

	/**
	 * @brief Get the file name of given module HANDLE
	 * @param[in] hModule 
	 * The HANDLE to the module where you want to get file name.
	 * It is same as the HANDLE parameter of Win32 \c GetModuleFileName.
	 * @param[out] ret The variable receiving UTF8 encoded file name of given module.
	 * @return True if success, otherwise false.
	*/
	bool GetModuleFileName(HINSTANCE hModule, yycc_u8string& ret);

	/**
	 * @brief Get the path to \%LOCALAPPDATA\%.
	 * @details \%LOCALAPPDATA\% usually was used as putting local app data files
	 * @param[out] ret The variable receiving UTF8 encoded path to LOCALAPPDATA.
	 * @return True if success, otherwise false.
	*/
	bool GetLocalAppData(yycc_u8string& ret);

	/**
	 * @brief Check whether given code page number is a valid one.
	 * @param[in] code_page The code page number.
	 * @return True if it is valid, otherwise false.
	*/
	bool IsValidCodePage(UINT code_page);

	/**
	 * @brief Copies an existing file to a new file.
	 * @param lpExistingFileName The name of an existing file.
	 * @param lpNewFileName The name of the new file.
	 * @param bFailIfExists 
	 * If this parameter is TRUE and the new file specified by \c lpNewFileName already exists, the function fails.
	 * If this parameter is FALSE and the new file already exists, the function overwrites the existing file and succeeds.
	 * @return 
	 * If the function succeeds, the return value is nonzero.
	 * If the function fails, the return value is zero. To get extended error information, call \c GetLastError.
	 * @remarks Same as Windows \c CopyFile: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-copyfilew
	*/
	BOOL CopyFile(const yycc_u8string_view& lpExistingFileName, const yycc_u8string_view& lpNewFileName, BOOL bFailIfExists);

	/**
	 * @brief Moves an existing file or a directory, including its children.
	 * @param lpExistingFileName The current name of the file or directory on the local computer.
	 * @param lpNewFileName 
	 * The new name for the file or directory. The new name must not already exist.
	 * A new file may be on a different file system or drive. A new directory must be on the same drive.
	 * @return 
	 * If the function succeeds, the return value is nonzero.
	 * If the function fails, the return value is zero. To get extended error information, call \c GetLastError.
	 * @remarks Same as Windows \c MoveFile: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-movefilew
	*/
	BOOL MoveFile(const yycc_u8string_view& lpExistingFileName, const yycc_u8string_view& lpNewFileName);

	/**
	 * @brief Deletes an existing file.
	 * @param lpFileName The name of the file to be deleted.
	 * @return 
	 * If the function succeeds, the return value is nonzero.
	 * If the function fails, the return value is zero. To get extended error information, call \c GetLastError.
	 * @remarks Same as Windows \c DeleteFile: https://learn.microsoft.com/e-us/windows/win32/api/winbase/nf-winbase-deletefile
	*/
	BOOL DeleteFile(const yycc_u8string_view& lpFileName);

}

#endif
