#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

namespace YYCC::ExceptionHelper {
	
	/**
	 * @brief Register unhandled exception handler
	 * @details
	 * This function will set an internal function as unhandled exception handler on Windows.
	 * \n
	 * When unhandled exception raised, 
	 * That internal function will output error stacktrace in standard output 
	 * and log file (located in temp folder), and also generate a dump file 
	 * in temp folder (for convenient debugging of developer when reporting bugs) if it can.
	 * \n
	 * This function usually is called at the start of program.
	 * @remarks This function is Windows only.
	*/
	void Register();
	/**
	 * @brief Unregister unhandled exception handler
	 * @details 
	 * The reverse operation of Register().
	 * \n
	 * This function and Register() should always be used as a pair.
	 * You must call this function if you have called Register() before.
	 * \n
	 * This function usually is called at the end of program.
	 * @remarks This function is Windows only.
	*/
	void Unregister();

}

#endif
