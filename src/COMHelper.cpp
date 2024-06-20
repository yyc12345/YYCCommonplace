#include "COMHelper.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

namespace YYCC::COMHelper {

	/**
	 * @brief The guard for initialize COM environment.
	 * @details This class will try initializing COM environment by calling CoInitialize when constructing,
	 * and it also will try uninitializing COM environment when destructing.
	 * If initialization failed, uninitialization will not be executed.
	*/
	class ComGuard {
	public:
		ComGuard() : m_HasInit(false) {
			HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
			if (SUCCEEDED(hr)) m_HasInit = true;
		}
		~ComGuard() {
			if (m_HasInit) {
				CoUninitialize();
			}
		}

		bool IsInitialized() const {
			return m_HasInit;
		}

	protected:
		bool m_HasInit;
	};

	/**
	 * @brief The instance of COM environment guard.
	 * @details Dialog related function need COM environment,
	 * so we need initializing COM environment when loading this module,
	 * and uninitializing COM environment when we no longer use this module.
	 * So we use a static instance in here.
	 * And make it be const so no one can change it.
	*/
	static const ComGuard c_ComGuard {};

	bool IsInitialized() {
		return c_ComGuard.IsInitialized();
	}

}

#endif