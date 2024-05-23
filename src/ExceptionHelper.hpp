#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

namespace YYCC::ExceptionHelper {
	
	/**
	 * @brief Register unhandled exception handler
	 * @detail This function frequently called at the start of program.
	*/
	void Register();
	/**
	 * @brief Unregiister unhandled exception handler
	 * @detail This function frequently called at the end of program.
	*/
	void Unregister();

}

#endif
