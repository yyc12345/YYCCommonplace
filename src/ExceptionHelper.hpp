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
	 * @brief The callback function prototype which will be called when unhandled exception happened after registering.
	 * @details
	 * During registering unhandled exception handler,
	 * caller can optionally provide a function pointer matching this prorotype to register.
	 * Then it will be called if unhandled exception hanppened.
	 * 
	 * This callback will provide 2 readonly arguments.
	 * First is the path to error log file.
	 * Second is the path to core dump file.
	 * These pathes may be empty if internal handler fail to create them.
	 * 
	 * This callback is convenient for programmer using an explicit way to tell user an exception happened.
	 * Because in default, handler will only write error log to \c stderr and file.
	 * It will be totally invisible on a GUI application.
	*/
	using ExceptionCallback = void(*)(const yycc_u8string& log_path, const yycc_u8string& coredump_path);

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
	 * @param[in] callback User defined callback called when unhandled exception happened. nullptr if no callback.
	*/
	void Register(ExceptionCallback callback = nullptr);
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

#if defined(YYCC_DEBUG_UE_FILTER)
	long __stdcall DebugCallUExceptionImpl(void*);
#endif

}

#endif
