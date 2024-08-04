#include "WinFctHelper.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include "EncodingHelper.hpp"
#include "COMHelper.hpp"

namespace YYCC::WinFctHelper {
	
	HMODULE GetCurrentModule() {
		// Reference: https://stackoverflow.com/questions/557081/how-do-i-get-the-hmodule-for-the-currently-executing-code
		HMODULE hModule = NULL;
		GetModuleHandleExW(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,	// get address and do not inc ref counter.
			(LPCWSTR)GetCurrentModule,
			&hModule);

		return hModule;
	}

	bool GetTempDirectory(yycc_u8string& ret) {
		// create wchar buffer for receiving the temp path.
		std::wstring wpath(MAX_PATH + 1u, L'\0');
		DWORD expected_size;

		// fetch temp folder
		while (true) {
			if ((expected_size = GetTempPathW(static_cast<DWORD>(wpath.size()), wpath.data())) == 0) {
				// failed, set to empty
				return false;
			}

			if (expected_size > static_cast<DWORD>(wpath.size())) {
				// buffer is too short, need enlarge and do fetching again
				wpath.resize(expected_size);
			} else {
				// ok. shrink to real length,  break while
				break;
			}
		}

		// resize result
		wpath.resize(expected_size);
		// convert to utf8 and return
		return YYCC::EncodingHelper::WcharToUTF8(wpath, ret);
	}

	bool GetModuleFileName(HINSTANCE hModule, yycc_u8string& ret) {
		// create wchar buffer for receiving the temp path.
		std::wstring wpath(MAX_PATH + 1u, L'\0');
		DWORD copied_size;

		while (true) {
			if ((copied_size = GetModuleFileNameW(hModule, wpath.data(), static_cast<DWORD>(wpath.size()))) == 0) {
				// failed, return
				return false;
			}

			// check insufficient buffer
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
				// buffer is not enough, enlarge it and try again.
				wpath.resize(wpath.size() + MAX_PATH);
			} else {
				// ok, break while
				break;
			}
		}
		
		// resize result
		wpath.resize(copied_size);
		// convert to utf8 and return
		return YYCC::EncodingHelper::WcharToUTF8(wpath, ret);
	}

	bool GetLocalAppData(yycc_u8string& ret) {
		// check whether com initialized
		if (!COMHelper::IsInitialized()) return false;

		// fetch path
		LPWSTR _known_path;
		HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_CREATE, NULL, &_known_path);
		if (FAILED(hr)) return false;
		COMHelper::SmartLPWSTR known_path(_known_path);

		// convert to utf8
		return YYCC::EncodingHelper::WcharToUTF8(known_path.get(), ret);
	}

	bool IsValidCodePage(UINT code_page) {
		CPINFOEXW cpinfo;
		return GetCPInfoExW(code_page, 0, &cpinfo);
	}

}

#endif
