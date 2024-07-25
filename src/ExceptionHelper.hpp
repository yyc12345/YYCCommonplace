#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

/**
 * @brief Windows specific unhandled exception processor.
 * @details
 * This namespace is Windows specific. On other platforms, the whole namespace is unavailable.
 * For how to utilize this namespace, please see \ref exception_helper.
 * 
*/
namespace YYCC::ExceptionHelper {
	
	/**
	 * @brief Register unhandled exception handler
	 * @details
	 * This function will set an internal function as unhandled exception handler on Windows.
	 * 
	 * When unhandled exception raised, 
	 * That internal function will output error stacktrace in standard output,
	 * and generate log file and dump file in \c \%APPDATA\%/CrashDumps folder if it is possible.
	 * (for convenient debugging of developer when reporting bugs.)
	 * 
	 * This function usually is called at the start of program.
	*/
	void Register();
	/**
	 * @brief Unregister unhandled exception handler
	 * @details 
	 * The reverse operation of Register().
	 * 
	 * This function and Register() should always be used as a pair.
	 * You must call this function to release reources if you have called Register().
	 * 
	 * This function usually is called at the end of program.
	*/
	void Unregister();

}

#endif
