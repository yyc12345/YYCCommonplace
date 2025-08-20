#pragma once
#include "../macro/os_detector.hpp"
#include "../macro/stl_detector.hpp"
#if defined(YYCC_OS_WINDOWS) && defined(YYCC_STL_MSSTL)

#include "../macro/class_copy_move.hpp"
#include "com.hpp"
#include <string>
#include <string_view>
#include <vector>
#include <initializer_list>
#include <expected>
#include <optional>

#include "import_guard_head.hpp"
#include <Windows.h>
#include <shlobj_core.h>
#include "import_guard_tail.hpp"

#define NS_YYCC_WINDOWS_COM ::yycc::windows::com

/**
 * @brief The namespace providing Windows universal dialog features.
 * @details
 * This namespace only available on Windows platform.
 * See also \ref dialog_helper.
*/
namespace yycc::windows::dialog {

    /// @brief The error occurs in this module.
    enum class DialogError {
        BadEncoding,     ///< Error occurs when perform encoding convertion.
        TooManyFilters,  ///< The size of file filters list is too large for Windows.
        IndexOverflow,   ///< Default filter index is too large for Windows.
        IndexOutOfRange, ///< Default filter index is out of range of filters list.
        NoSuchDir,       ///< Given initial directory path is invalid.
        BadCOMCall,      ///< Some COM function calls failed.
    };

    /// @brief The result type used in this module.
    template<typename T>
    using DialogResult = std::expected<T, DialogError>;

    /**
     * @private
	 * @brief The class representing the file types region in file dialog.
	 * @details 
	 * This class is private and served for Windows used. 
	 * Programmer should \b not create this class manually.
	*/
    class WinFileFilters {
        friend class FileFilters;

    public:
        using WinFilterModes = std::wstring;
        using WinFilterName = std::wstring;
        using WinFilterPair = std::pair<WinFilterName, WinFilterModes>;

    public:
        WinFileFilters();
        ~WinFileFilters();
        YYCC_DECL_COPY_MOVE(WinFileFilters)

    public:
        /**
         * @brief Get the count of available file filters
         * @return Count of file filters.
         */
        UINT get_filter_count() const;
        /**
         * @brief Get pointer to Windows used file filters declarations
         * @return Pointer for Windows use.
         */
        const COMDLG_FILTERSPEC* get_filter_specs() const;

    private:
        /**
         * @brief Update COMDLG_FILTERSPEC according to file filter list.
         * @remarks Programmer \b MUST call this function after you modify m_WinFilters.
         */
        void update();

    private:
        std::vector<WinFilterPair> m_WinFilters;
        std::vector<COMDLG_FILTERSPEC> m_WinDataStruct;
    };

    /**
	 * @brief The class representing the file types region in file dialog.
	 * @details 
	 * This class is served for programmer using.
	 * But you don't need create it on your own.
	 * You can simply fetch it by FileDialog::ConfigreFileTypes(),
	 * because this class is a part of FileDialog.
	*/
    class FileFilters {
    public:
        using FilterModes = std::vector<std::u8string>;
        using FilterName = std::u8string;
        using FilterPair = std::pair<FilterName, FilterModes>;

    public:
        FileFilters();
        ~FileFilters();
        YYCC_DEFAULT_COPY_MOVE(FileFilters)

    public:
        /**
		 * @brief Add a filter pair in file types list.
		 * @param[in] filter_name The friendly name of the filter.
		 * @param[in] il 
		 * A C++ initialize list containing acceptable file filter pattern.
		 * Every entries must be a string representing a single filter pattern.
		 * This list at least should have one pattern.
		 * @return True if added success, otherwise false.
         * @warning This function will not validate the content of these filter patterns, so please write them carefully.
         * @exception std::invalid_argument Given filtern name is blank, or filter patterns is empty.
		*/
        void add_filter(const std::u8string_view& filter_name, std::initializer_list<std::u8string_view> il);
        /**
         * @brief Get the count of added file filters.
         * @return The count of added file filters.
         */
        size_t get_count() const;
        /**
         * @brief Clear filter pairs for following re-use.
         */
        void clear();

        /**
         * @private
         * @brief Build Windows used file filters struct.
         * @return Built Windows used struct, or error occurs.
         */
        DialogResult<WinFileFilters> to_windows() const;

    private:
        std::vector<FilterPair> m_Filters;
    };

    /**
     * @private
	 * @brief The class representing the file dialog.
	 * @details 
	 * This class is served for Windows used. 
	 * Programmer should \b not create this class manually.
	*/
    class WinFileDialog {
        friend class FileDialog;

    public:
        WinFileDialog();
        ~WinFileDialog();
        YYCC_DECL_COPY(WinFileDialog)
        YYCC_DEFAULT_MOVE(WinFileDialog)

        /// @brief Get whether this dialog has owner.
        bool has_owner() const;
        /// @brief Get the \c HWND of dialog owner.
        HWND get_owner() const;
        /// @brief Get whether dialog has custom title.
        bool has_title() const;
        /// @brief Get custom title of dialog.
        const wchar_t* get_title() const;
        /// @brief Get whether dialog has custom initial file name.
        bool has_init_file_name() const;
        /// @brief Get custom initial file name of dialog
        const wchar_t* get_init_file_name() const;
        /// @brief Get whether dialog has custom initial directory.
        bool has_init_directory() const;
        /// @brief Get custom initial directory of dialog.
        IShellItem* get_init_directory() const;
        /// @brief Get the struct holding Windows used file filters data.
        const WinFileFilters& get_file_types() const;
        /// @brief Get the index of default selected file filter.
        UINT get_default_file_type_index() const;

    private:
        HWND m_WinOwner;
        WinFileFilters m_WinFileTypes;
        /**
		 * @brief The default selected file type in dialog
		 * @remarks 
		 * This is 1-based index according to Windows specification.
		 * In other words, when generating this struct from FileDialog to this struct this field should plus 1.
		 * Because the same field located in FileDialog is 0-based index.
		*/
        UINT m_WinDefaultFileTypeIndex;
        std::optional<std::wstring> m_WinTitle, m_WinInitFileName;
        NS_YYCC_WINDOWS_COM::SmartIShellItem m_WinInitDirectory;
    };

    /**
	 * @brief The class representing the file dialog.
	 * @details 
	 * This class is served for programming using to describe every aspectes of the dialog.
	 * For how to use this struct, see \ref dialog_helper.
	*/
    class FileDialog {
    public:
        FileDialog();
        ~FileDialog();
        YYCC_DEFAULT_COPY_MOVE(FileDialog)

        /**
		 * @brief Set the owner of dialog.
		 * @param[in] owner The \c HWND pointing to the owner of dialog, or NULL to remove owner.
		*/
        void set_owner(HWND owner);
        /**
		 * @brief Set custom title of dialog
		 * @param[in] title The string pointer to custom title.
		*/
        void set_title(const std::u8string_view& title);
        /**
         * @brief Remove custom title of dialog (keep system default)
         */
        void unset_title();
        /**
		 * @brief Set the initial file name of dialog
		 * @details If set, the file name will always be same one when opening dialog.
		 * @param[in] init_filename String pointer to initial file name.
		*/
        void set_init_file_name(const std::u8string_view& init_filename);
        /**
         * @brief Remove custom initial file name of dialog (keep system default)
         */
        void unset_init_file_name();
        /**
		 * @brief Set the initial directory of dialog
		 * @details If set, the opended directory will always be the same one when opening dialog
		 * @param[in] init_dir String pointer to initial directory.
		*/
        void set_init_directory(const std::u8string_view& init_dir);
        /**
         * @brief Remove custom initial directory of dialog (keep system default)
         */
        void unset_init_directory();
        /**
		 * @brief Fetch the struct describing file filters for future configuration.
		 * @return The reference to the struct describing file filters.
		*/
        FileFilters& configre_file_types();
        /**
		 * @brief Set the index of default selected file filter.
		 * @param[in] idx 
		 * The index to default file filter.
		 * This must be a valid index in file filters.
		*/
        void set_default_file_type_index(size_t idx);
        /**
         * @brief Clear file dialog parameters for following re-use.
         */
        void clear();

        /**
         * @private
		 * @brief Build Windows used dialog info struct.
		 * @return Built Windows used struct, or error occurs.
		*/
        DialogResult<WinFileDialog> to_windows() const;

    protected:
        HWND m_Owner;
        std::optional<std::u8string> m_Title, m_InitFileName, m_InitDirectory;
        FileFilters m_FileTypes;
        /**
		 * @brief The default selected file type in dialog
		 * @remarks 
		 * The index Windows used is 1-based index.
		 * But for universal experience, we order this is 0-based index.
		 * And do convertion when generating Windows used struct.
		*/
        size_t m_DefaultFileTypeIndex;
    };

    /**
     * @brief The result after user close the dialog.
     * @details
     * It is an alias to \c std::optional.
     * If it do not have value, it means that user click "Cancel" button.
     * otherwise it contain a value that user selected in dialog.
     */
    template<typename T>
    using DialogOutcome = std::optional<T>;

    /**
	 * @brief Open the dialog which order user select single file to open.
	 * @param[in] params The configuration of dialog.
	 * @return Full path to user selected file, or nothing (click "Cancel"), or error occurs.
	*/
    DialogResult<DialogOutcome<std::u8string>> open_file(const FileDialog& params);
    /**
	 * @brief Open the dialog which order user select multiple file to open.
	 * @param[in] params The configuration of dialog.
	 * @return The list of full path of user selected files, or nothing (click "Cancel"), or error occurs.
	*/
    DialogResult<DialogOutcome<std::vector<std::u8string>>> open_files(const FileDialog& params);
    /**
	 * @brief Open the dialog which order user select single file to save.
	 * @param[in] params The configuration of dialog.
	 * @return Full path to user selected file, or nothing (click "Cancel"), or error occurs.
	*/
    DialogResult<DialogOutcome<std::u8string>> save_file(const FileDialog& params);
    /**
	 * @brief Open the dialog which order user select single directory to open.
	 * @param[in] params The configuration of dialog.
	 * @return Full path to user selected directory, or nothing (click "Cancel"), or error occurs.
	*/
    DialogResult<DialogOutcome<std::u8string>> open_folder(const FileDialog& params);

}

#undef NS_YYCC_WINDOWS_COM

#endif
