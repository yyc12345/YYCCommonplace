#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

/**
 * @brief Windows specific unhandled exception processor.
 * @details
 * This namespace is Windows specific. On other platforms, the whole namespace is unavailable.
 * 
 * This namespace allow user register unhandled exception handler on Windows
 * to output error log into \c stderr and log file, and generate coredump if possible.
 * This is useful for bug tracing on Windows, especially most Windows user are naive and don't know how to report bug.
 * 
*/
namespace YYCC::ExceptionHelper {
	
	/**
	 * @brief Register unhandled exception handler
	 * @details
	 * This function will set an internal function as unhandled exception handler on Windows.
	 * 
	 * When unhandled exception raised, 
	 * That internal function will output error stacktrace in standard output 
	 * and log file (located in temp folder), and also generate a dump file 
	 * in temp folder (for convenient debugging of developer when reporting bugs) if it can.
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
	 * You must call this function if you have called Register() before.
	 * 
	 * This function usually is called at the end of program.
	*/
	void Unregister();

}

#endif
