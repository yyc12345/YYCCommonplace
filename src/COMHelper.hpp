#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include <memory>

#include "WinImportPrefix.hpp"
#include <Windows.h>
#include <shlobj_core.h>
#include "WinImportSuffix.hpp"

/**
 * @brief COM fucntions related namespace.
 * @details
 * This namespace is Windows specific and is unavailable on other platforms.
 * 
 * This namespace contain a COM Guard which make sure COM was initialized in current module when loading current module.
 * It is essential because all calling to COM functions should be under the premise that COM has been initialized.
 * This guard also will uninitialize COM when unloading this module.
 * 
 * This namespace also provided various memory-safe types for interacting with COM functions.
 * Although Microsoft also has similar smart pointer called \c CComPtr.
 * But this library is eager to hide all Microsoft-related functions calling.
 * Using \c CComPtr is not corresponding with the philosophy of this library.
 * So these std-based smart pointer type were created.
 * 
 * This namespace is used by internal functions as intended.
 * They should not be used outside of this library.
 * But if you compel to use them, it is also okey.
*/
namespace YYCC::COMHelper {

	/**
	 * @brief C++ standard deleter for every COM interfaces inheriting IUnknown.
	*/
	class ComPtrDeleter {
	public:
		ComPtrDeleter() {}
		void operator() (IUnknown* com_ptr) {
			if (com_ptr != nullptr) {
				com_ptr->Release();
			}
		}
	};

	using SmartIFileDialog = std::unique_ptr<IFileDialog, ComPtrDeleter>;
	using SmartIFileOpenDialog = std::unique_ptr<IFileOpenDialog, ComPtrDeleter>;
	using SmartIShellItem = std::unique_ptr<IShellItem, ComPtrDeleter>;
	using SmartIShellItemArray = std::unique_ptr<IShellItemArray, ComPtrDeleter>;
	using SmartIShellFolder = std::unique_ptr<IShellFolder, ComPtrDeleter>;

	/**
	 * @brief C++ standard deleter for almost raw pointer used in COM which need to be free by CoTaskMemFree()
	*/
	class CoTaskMemDeleter {
	public:
		CoTaskMemDeleter() {}
		void operator() (void* com_ptr) {
			if (com_ptr != nullptr) {
				CoTaskMemFree(com_ptr);
			}
		}
	};

	using SmartLPWSTR = std::unique_ptr<std::remove_pointer_t<LPWSTR>, CoTaskMemDeleter>;

}

#endif
