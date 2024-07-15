#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include <memory>

#include "WinImportPrefix.hpp"
#include <Windows.h>
#include <shlobj_core.h>
#include "WinImportSuffix.hpp"

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

	/**
	 * @brief Check whether COM environment has been initialized.
	 * @return True if it is, otherwise false.
	 * @remarks
	 * This function will call corresponding function of COM Guard.
	 * Do not remove this function and you must preserve at least one reference to this function in final program.
	 * Some compiler will try to drop COM Guard in final program if no reference to it and it will cause the initialization of COM environment failed.
	 * This is the reason why I order you do the things said above.
	*/
	bool IsInitialized();

}

#endif
