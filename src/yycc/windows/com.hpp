#pragma once
#include "../macro/os_detector.hpp"
#include "../macro/stl_detector.hpp"
#if defined(YYCC_OS_WINDOWS) && defined(YYCC_STL_MSSTL)

#include <memory>

#include "import_guard_head.hpp"
#include <Windows.h>
#include <shlobj_core.h>
#include "import_guard_tail.hpp"

/**
 * @brief Windows COM related types and checker.
 * @details
 * This namespace is Windows specific.
 * In other platforms, this whole namespace will be unavailable.
*/
namespace yycc::windows::com {

    /// @brief C++ standard deleter for every COM interfaces inheriting IUnknown.
    class ComPtrDeleter {
    public:
        ComPtrDeleter() {}
        void operator()(IUnknown* com_ptr) {
            if (com_ptr != nullptr) {
                com_ptr->Release();
            }
        }
    };

    /// @brief Smart unique pointer of \c IFileDialog
    using SmartIFileDialog = std::unique_ptr<IFileDialog, ComPtrDeleter>;
    /// @brief Smart unique pointer of \c IFileOpenDialog
    using SmartIFileOpenDialog = std::unique_ptr<IFileOpenDialog, ComPtrDeleter>;
    /// @brief Smart unique pointer of \c IShellItem
    using SmartIShellItem = std::unique_ptr<IShellItem, ComPtrDeleter>;
    /// @brief Smart unique pointer of \c IShellItemArray
    using SmartIShellItemArray = std::unique_ptr<IShellItemArray, ComPtrDeleter>;
    /// @brief Smart unique pointer of \c IShellFolder
    using SmartIShellFolder = std::unique_ptr<IShellFolder, ComPtrDeleter>;

    /// @brief C++ standard deleter for almost raw pointer used in COM which need to be free by CoTaskMemFree()
    class CoTaskMemDeleter {
    public:
        CoTaskMemDeleter() {}
        void operator()(void* com_ptr) {
            if (com_ptr != nullptr) {
                CoTaskMemFree(com_ptr);
            }
        }
    };

    /// @brief Smart unique pointer of COM created \c WCHAR sequence.
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
    bool is_initialized();

} // namespace yycc::windows::com

#endif
