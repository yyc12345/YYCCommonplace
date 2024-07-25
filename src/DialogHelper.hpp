#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include "COMHelper.hpp"
#include <string>
#include <vector>
#include <initializer_list>

#include "WinImportPrefix.hpp"
#include <Windows.h>
#include <shlobj_core.h>
#include "WinImportSuffix.hpp"

/**
 * @brief The namespace providing Windows universal dialog features.
 * @details
 * This namespace only available on Windows platform.
 * See also \ref dialog_helper.
*/
namespace YYCC::DialogHelper {

	/**
	 * @brief The class representing the file types region in file dialog.
	 * @details 
	 * This class is served for Windows used. 
	 * Programmer should \b not create this class manually.
	*/
	class WinFileFilters {
		friend class FileFilters;
		friend class WinFileDialog;
	public:
		WinFileFilters() : m_WinFilters(), m_WinDataStruct(nullptr) {}

		/// @brief Get the count of available file filters
		UINT GetFilterCount() const {
			return static_cast<UINT>(m_WinFilters.size());
		}
		/// @brief Get pointer to Windows used file filters declarations
		const COMDLG_FILTERSPEC* GetFilterSpecs() const {
			return m_WinDataStruct.get();
		}

	protected:
		using WinFilterModes = std::wstring;
		using WinFilterName = std::wstring;
		using WinFilterPair = std::pair<WinFilterName, WinFilterModes>;

		std::vector<WinFilterPair> m_WinFilters;
		std::unique_ptr<COMDLG_FILTERSPEC[]> m_WinDataStruct;

		/// @brief Clear all current file filters
		void Clear() {
			m_WinDataStruct.reset();
			m_WinFilters.clear();
		}
	};

	/**
	 * @brief The class representing the file types region in file dialog.
	 * @details 
	 * This class is served for programmer using.
	 * But you don't need create it on your own.
	 * You can simply fetch it by FileDialog::ConfigreFileTypes ,
	 * because this class is a part of FileDialog.
	*/
	class FileFilters {
	public:
		FileFilters() : m_Filters() {}

		/**
		 * @brief Add a filter pair in file types list.
		 * @param[in] filter_name The friendly name of the filter.
		 * @param[in] il 
		 * A C++ initialize list containing acceptable file filter pattern.
		 * Every entries must be `const yycc_char8_t*` representing a single filter pattern.
		 * The list at least should have one valid pattern.
		 * This function will not validate these filter patterns, so please write them carefully.
		 * @return True if added success, otherwise false.
		 * @remarks
		 * This function allow you register multiple filter patterns for single friendly name.
		 * For example: <TT>Add(u8"Microsoft Word (*.doc; *.docx)", {u8"*.doc", u8"*.docx"})</TT>
		*/
		bool Add(const yycc_char8_t* filter_name, std::initializer_list<const yycc_char8_t*> il);
		/**
		 * @brief Get the count of added filter pairs.
		 * @return The count of already added filter pairs.
		*/
		size_t Count() const { return m_Filters.size(); }

		/// @brief Clear filter pairs for following re-use.
		void Clear() { m_Filters.clear(); }

		/**
		 * @brief Generate Windows dialog system used data struct.
		 * @param[out] win_result The class receiving the generated filter data struct.
		 * @return True if generation success, otherwise false.
		 * @remarks 
		 * Programmer should not call this function, 
		 * this function is used as YYCC internal code.
		*/
		bool Generate(WinFileFilters& win_result) const;

	protected:
		using FilterModes = std::vector<yycc_u8string>;
		using FilterName = yycc_u8string;
		using FilterPair = std::pair<FilterName, FilterModes>;

		std::vector<FilterPair> m_Filters;
	};

	/**
	 * @brief The class representing the file dialog.
	 * @details 
	 * This class is served for Windows used. 
	 * Programmer should \b not create this class manually.
	*/
	class WinFileDialog {
		friend class FileDialog;
	public:
		WinFileDialog() :
			m_WinOwner(NULL),
			m_WinFileTypes(), m_WinDefaultFileTypeIndex(0u),
			m_HasTitle(false), m_HasInitFileName(false), m_WinTitle(), m_WinInitFileName(),
			m_WinInitDirectory(nullptr) {}

		/// @brief Get whether this dialog has owner.
		bool HasOwner() const { return m_WinOwner != NULL; }
		/// @brief Get the \c HWND of dialog owner.
		HWND GetOwner() const { return m_WinOwner; }
		
		/// @brief Get the struct holding Windows used file filters data.
		const WinFileFilters& GetFileTypes() const { return m_WinFileTypes; }
		/// @brief Get the index of default selected file filter.
		UINT GetDefaultFileTypeIndex() const { return m_WinDefaultFileTypeIndex; }
		
		/// @brief Get whether dialog has custom title.
		bool HasTitle() const { return m_HasTitle; }
		/// @brief Get custom title of dialog.
		const wchar_t* GetTitle() const { return m_WinTitle.c_str(); }
		/// @brief Get whether dialog has custom initial file name.
		bool HasInitFileName() const { return m_HasInitFileName; }
		/// @brief Get custom initial file name of dialog
		const wchar_t* GetInitFileName() const { return m_WinInitFileName.c_str(); }
		
		/// @brief Get whether dialog has custom initial directory.
		bool HasInitDirectory() const { return m_WinInitDirectory.get() != nullptr; }
		/// @brief Get custom initial directory of dialog.
		IShellItem* GetInitDirectory() const { return m_WinInitDirectory.get(); }

	protected:
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
		bool m_HasTitle, m_HasInitFileName;
		std::wstring m_WinTitle, m_WinInitFileName;
		COMHelper::SmartIShellItem m_WinInitDirectory;
		
		/// @brief Clear all data and reset them to default value.
		void Clear() {
			m_WinOwner = nullptr;
			m_WinFileTypes.Clear();
			m_WinDefaultFileTypeIndex = 0u;
			m_HasTitle = m_HasInitFileName = false;
			m_WinTitle.clear();
			m_WinInitFileName.clear();
			m_WinInitDirectory.reset();
		}
	};

	/**
	 * @brief The class represent the file dialog.
	 * @details 
	 * This class is served for programming using to describe every aspectes of the dialog.
	 * For how to use this struct, see \ref dialog_helper.
	*/
	class FileDialog {
	public:
		FileDialog() :
			m_Owner(NULL),
			m_FileTypes(),
			m_DefaultFileTypeIndex(0u),
			m_Title(), m_InitFileName(), m_InitDirectory(),
			m_HasTitle(false), m_HasInitFileName(false), m_HasInitDirectory(false) {}

		/**
		 * @brief Set the owner of dialog.
		 * @param[in] owner The \c HWND pointing to the owner of dialog, or NULL to remove owner.
		*/
		void SetOwner(HWND owner) { m_Owner = owner; }
		/**
		 * @brief Set custom title of dialog
		 * @param[in] title The string pointer to custom title, or nullptr to remove it.
		*/
		void SetTitle(const yycc_char8_t* title) {
			if (m_HasTitle = title != nullptr)
				m_Title = title;
		}
		/**
		 * @brief Fetch the struct describing file filters for future configuration.
		 * @return The reference to the struct describing file filters.
		*/
		FileFilters& ConfigreFileTypes() {
			return m_FileTypes;
		}
		/**
		 * @brief Set the index of default selected file filter.
		 * @param[in] idx 
		 * The index to default one.
		 * This must be a valid index in file filters.
		*/
		void SetDefaultFileTypeIndex(size_t idx) { m_DefaultFileTypeIndex = idx; }
		/**
		 * @brief Set the initial file name of dialog
		 * @details If set, the file name will always be same one when opening dialog.
		 * @param[in] init_filename String pointer to initial file name, or nullptr to remove it.
		*/
		void SetInitFileName(const yycc_char8_t* init_filename) {
			if (m_HasInitFileName = init_filename != nullptr)
				m_InitFileName = init_filename;
		}
		/**
		 * @brief Set the initial directory of dialog
		 * @details If set, the opended directory will always be the same one when opening dialog
		 * @param[in] init_dir 
		 * String pointer to initial directory.
		 * Invalid path or nullptr will remove this feature.
		*/
		void SetInitDirectory(const yycc_char8_t* init_dir) {
			if (m_HasInitDirectory = init_dir != nullptr)
				m_InitDirectory = init_dir;
		}

		/// @brief Clear file dialog parameters for following re-use.
		void Clear() {
			m_Owner = nullptr;
			m_HasTitle = m_HasInitFileName = m_HasInitDirectory = false;
			m_Title.clear();
			m_InitFileName.clear();
			m_InitDirectory.clear();
			m_FileTypes.Clear();
			m_DefaultFileTypeIndex = 0u;
		}
		
		/**
		 * @brief Generate Windows dialog system used data struct.
		 * @param[out] win_result The class receiving the generated filter data struct.
		 * @return True if generation is success, otherwise false.
		 * @remarks 
		 * Programmer should not call this function.
		 * This function is used as YYCC internal code.
		*/
		bool Generate(WinFileDialog& win_result) const;

	protected:
		HWND m_Owner;
		bool m_HasTitle, m_HasInitFileName, m_HasInitDirectory;
		yycc_u8string m_Title, m_InitFileName, m_InitDirectory;
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
	 * @brief Open the dialog which order user select single file to open.
	 * @param[in] params The configuration of dialog.
	 * @param[out] ret Full path to user selected file.
	 * @return False if user calcel the operation or something went wrong, otherwise true.
	*/
	bool OpenFileDialog(const FileDialog& params, yycc_u8string& ret);
	/**
	 * @brief Open the dialog which order user select multiple file to open.
	 * @param[in] params The configuration of dialog.
	 * @param[out] ret The list of full path of user selected files.
	 * @return False if user calcel the operation or something went wrong, otherwise true.
	*/
	bool OpenMultipleFileDialog(const FileDialog& params, std::vector<yycc_u8string>& ret);
	/**
	 * @brief Open the dialog which order user select single file to save.
	 * @param[in] params The configuration of dialog.
	 * @param[out] ret Full path to user selected file.
	 * @return False if user calcel the operation or something went wrong, otherwise true.
	*/
	bool SaveFileDialog(const FileDialog& params, yycc_u8string& ret);
	/**
	 * @brief Open the dialog which order user select single directory to open.
	 * @param[in] params The configuration of dialog.
	 * @param[out] ret Full path to user selected directory.
	 * @return False if user calcel the operation or something went wrong, otherwise true.
	*/
	bool OpenFolderDialog(const FileDialog& params, yycc_u8string& ret);

}

#endif
