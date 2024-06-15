#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include <string>

#include "WinImportPrefix.hpp"
#include <Windows.h>
#include "WinImportSuffix.hpp"

/**
 * @brief The helper providing assistance to Win32 functions.
 * @details
 * This helper is Windows specific.
 * If current environment is not Windows, 
 * the whole namespace will disappear.
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
	 * For example, you may want to get the name of your built DLL at runtime, then you should pass current module HANDLE, not the HANDLE of EXE.
	 * Or, if you want to get the path to your DLL, you also should pass current module HANDLE.
	 * @return A Windows HANDLE pointing to current module, NULL if failed.
	*/
	HMODULE GetCurrentModule();

	/**
	 * @brief Get path to Windows temp folder.
	 * @param[out] ret 
	 * The variable receiving UTF8 encoded path to Windows temp folder.
	 * @return True if success, otherwise false.
	*/
	bool GetTempDirectory(std::string& ret);

	/**
	 * @brief Get the file name of given module HANDLE
	 * @param[in] hModule 
	 * The HANDLE to the module where we want get file name.
	 * It is same as the HANDLE parameter of \c GetModuleFileName.
	 * @param[out] ret 
	 * The variable receiving UTF8 encoded file name of given module.
	 * @return True if success, otherwise false.
	*/
	bool GetModuleName(HINSTANCE hModule, std::string& ret);
}

#endif
