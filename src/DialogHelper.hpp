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

namespace YYCC::DialogHelper {

	/**
	 * @brief The class represent the file types region in file dialog
	 * @details THis class is specific for Windows use, not user oriented.
	*/
	class WinFileFilters {
		friend class FileFilters;
		friend class WinFileDialog;
	public:
		WinFileFilters() : m_WinFilters(), m_WinDataStruct(nullptr) {}

		UINT GetFilterCount() const {
			return static_cast<UINT>(m_WinFilters.size());
		}
		const COMDLG_FILTERSPEC* GetFilterSpecs() const {
			return m_WinDataStruct.get();
		}

	protected:
		using WinFilterModes = std::wstring;
		using WinFilterName = std::wstring;
		using WinFilterPair = std::pair<WinFilterName, WinFilterModes>;

		std::vector<WinFilterPair> m_WinFilters;
		std::unique_ptr<COMDLG_FILTERSPEC[]> m_WinDataStruct;

		void Clear() {
			m_WinDataStruct.reset();
			m_WinFilters.clear();
		}
	};

	/**
	 * @brief The class represent the file types region in file dialog.
	 * @details This class is user oriented. User can use function manipulate file types
	 * and final generation function will produce Windows-understood data struct from this.
	*/
	class FileFilters {
	public:
		FileFilters() : m_Filters() {}

		/**
		 * @brief Add a filter pair in file types list.
		 * @param filter_name[in] The friendly name of the filter.
		 * @param il[in] A C++ initialize list.
		 * Every entries must be `const char*` represent a single filter pattern.
		 * The list at least should have one valid pattern.
		 * This function will not validate these filter patterns, so please write them carefully.
		 * @return True if added success, otherwise false.
		 * @remarks This function allow you register multiple filter patterns for single friendly name.
		 * For example: `Add("Microsoft Word (*.doc; *.docx)", {"*.doc", "*.docx"})`
		*/
		bool Add(const char* filter_name, std::initializer_list<const char*> il);
		/**
		 * @brief Clear filter pairs for following re-use.
		*/
		void Clear() { m_Filters.clear(); }
		/**
		 * @brief Get the count of added filter pairs.
		 * @return The count of already added filter pairs.
		*/
		size_t Count() const { return m_Filters.size(); }

		/**
		 * @brief Generate Windows dialog system used data struct.
		 * @param win_result[out] The class holding the generated filter data struct.
		 * @return True if generation is success, otherwise false.
		 * @remarks User should not call this function, this function is used in internal code.
		*/
		bool Generate(WinFileFilters& win_result) const;

	protected:
		using FilterModes = std::vector<std::string>;
		using FilterName = std::string;
		using FilterPair = std::pair<FilterName, FilterModes>;

		std::vector<FilterPair> m_Filters;
	};

	/**
	 * @brief The class represent the file dialog
	 * @details THis class is specific for Windows use, not user oriented.
	*/
	class WinFileDialog {
		friend class FileDialog;
	public:
		WinFileDialog() :
			m_WinOwner(NULL),
			m_WinFileTypes(), m_WinDefaultFileTypeIndex(0u),
			m_HasTitle(false), m_HasInitFileName(false), m_WinTitle(), m_WinInitFileName(),
			m_WinInitDirectory(nullptr) {}

		bool HasOwner() const { return m_WinOwner != NULL; }
		HWND GetOwner() const { return m_WinOwner; }

		const WinFileFilters& GetFileTypes() const { return m_WinFileTypes; }
		UINT GetDefaultFileTypeIndex() const { return m_WinDefaultFileTypeIndex; }

		bool HasTitle() const { return m_HasTitle; }
		const wchar_t* GetTitle() const { return m_WinTitle.c_str(); }
		bool HasInitFileName() const { return m_HasInitFileName; }
		const wchar_t* GetInitFileName() const { return m_WinInitFileName.c_str(); }

		bool HasInitDirectory() const { return m_WinInitDirectory.get() != nullptr; }
		IShellItem* GetInitDirectory() const { return m_WinInitDirectory.get(); }

	protected:
		HWND m_WinOwner;
		WinFileFilters m_WinFileTypes;
		/**
		 * @brief The default selected file type in dialog
		 * @remarks This is 1-based index according to Windows specification.
		 * In other words, you should plus 1 for this index when generating this struct from
		 * user oriented file dialog parameters.
		*/
		UINT m_WinDefaultFileTypeIndex;
		bool m_HasTitle, m_HasInitFileName;
		std::wstring m_WinTitle, m_WinInitFileName;
		COMHelper::SmartIShellItem m_WinInitDirectory;

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
	 * @details This class is user oriented. User can use function manipulate file dialog properties
	 * and final generation function will produce Windows-understood data struct from this.
	*/
	class FileDialog {
	public:
		FileDialog() :
			m_Owner(NULL),
			m_FileTypes(),
			m_DefaultFileTypeIndex(0u),
			m_Title(), m_InitFileName(), m_InitDirectory(),
			m_HasTitle(false), m_HasInitFileName(false), m_HasInitDirectory(false) {}

		void SetOwner(HWND owner) { m_Owner = owner; }
		void SetTitle(const char* title) {
			if (m_HasTitle = title != nullptr)
				m_Title = title;
		}
		FileFilters& ConfigreFileTypes() {
			return m_FileTypes;
		}
		void SetDefaultFileTypeIndex(size_t idx) { m_DefaultFileTypeIndex = idx; }
		void SetInitFileName(const char* init_filename) {
			if (m_HasInitFileName = init_filename != nullptr)
				m_InitFileName = init_filename;
		}
		void SetInitDirectory(const char* init_dir) {
			if (m_HasInitDirectory = init_dir != nullptr)
				m_InitDirectory = init_dir;
		}

		/**
		 * @brief Clear file dialog parameters for following re-use.
		*/
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
		 * @param win_result[out] The class holding the generated filter data struct.
		 * @return True if generation is success, otherwise false.
		 * @remarks User should not call this function, this function is used in internal code.
		*/
		bool Generate(WinFileDialog& win_result) const;

	protected:
		HWND m_Owner;
		bool m_HasTitle, m_HasInitFileName, m_HasInitDirectory;
		std::string m_Title, m_InitFileName, m_InitDirectory;
		FileFilters m_FileTypes;
		/**
		 * @brief The default selected file type in dialog
		 * @remarks Although Windows notice that this is a 1-based index,
		 * but for universal experience, we order this is 0-based index.
		*/
		size_t m_DefaultFileTypeIndex;
	};

	bool OpenFileDialog(const FileDialog& params, std::string& ret);
	bool OpenMultipleFileDialog(const FileDialog& params, std::vector<std::string>& ret);
	bool SaveFileDialog(const FileDialog& params, std::string& ret);

	bool OpenFolderDialog(const FileDialog& params, std::string& ret);

}

#endif
