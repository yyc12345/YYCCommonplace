#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include <string>
#include <vector>
#include <initializer_list>
#include <memory>

#include "WinImportPrefix.hpp"
#include <Windows.h>
#include <shlobj_core.h>
#include <commdlg.h>
#include "WinImportSuffix.hpp"

namespace YYCC::DialogHelper {

	/**
	 * @brief The class represent the file types region in file dialog.
	*/
	class FileFilters {
	public:
		FileFilters() : 
			m_Filters(),
			m_WinFilters(), m_WinDataStruct(nullptr)
		{}

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
		void Clear();

		/**
		 * @brief Generate Windows dialog system used data struct.
		 * @param filter_count[out] The count of generated filter data struct.
		 * @param filter_specs[out] The pointer to generated filter data struct.
		 * @remarks User should not call this function. This function is used by internal functions.
		 * @return True if generation is success, otherwise false.
		*/
		bool Generate(UINT& filter_count, COMDLG_FILTERSPEC*& filter_specs);

	protected:
		using FilterModes = std::vector<std::string>;
		using FilterName = std::string;
		using FilterPair = std::pair<FilterName, FilterModes>;

		std::vector<FilterPair> m_Filters;

		using WinFilterModes = std::wstring;
		using WinFilterName = std::wstring;
		using WinFilterPair = std::pair<WinFilterName, WinFilterModes>;

		std::vector<WinFilterPair> m_WinFilters;
		std::unique_ptr<COMDLG_FILTERSPEC[]> m_WinDataStruct;
	};

	//struct FileDialogParameter {
	//	FileDialogParameter() :
	//		m_Owner(nullptr),
	//		m_Filter(), m_SelectedFilter(0),
	//		m_Title(),
	//		m_DefaultExtension(), m_InitialDirectory(), m_InitialFileName() {}

	//	HWND m_Owner;
	//	std::vector<std::pair<std::string, std::string>> m_Filter;
	//	size_t m_SelectedFilter;
	//	std::string m_Title;
	//	std::string m_DefaultExtension;
	//	std::string m_InitialFileName;
	//	std::string m_InitialDirectory;
	//};

	//struct FolderDialogParameter {
	//	FolderDialogParameter() :
	//		m_Owner(nullptr),
	//		m_Title() {}

	//	HWND m_Owner;
	//	std::string m_Title;
	//};

	//bool OpenFileDialog(const FileDialogParameter& params, std::string& ret);
	//bool OpenMultipleFileDialog(const FileDialogParameter& params, std::vector<std::string>& ret);
	//bool SaveFileDialog(const FileDialogParameter& params, std::string& ret);

	//bool OpenFolderDialog(const FolderDialogParameter& params, std::string& ret);

}

#endif
