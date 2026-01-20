#pragma once
#include <string>
#include <string_view>
#include <optional>

/**
 * @brief Windows specific unhandled exception handler.
 * @details
 * This namespace is currently works on Windows.
 * On other platforms, this namespace provided functions do nothing.
 * For how to utilize this namespace, please see \ref ironpad.
 * 
 * This feature is originate from my created Virtools plugin.
 * Because its user frequently trigger some weird behaviors but I have no idea about the detail of them.
 * So I create this feature. So that I can order user upload error log and coredump to help my debugging.
 * The original implementation of this feature is copied from chirs241097's open source project whose name I forgotten.
 * 
 * After that, I split it from my plugin and let it become an independent library call IronPad,
 * and use it in libcmo21 and etc.
 * After few months, I created first version of YYCC and I move it into it and rename it as Exception Helper.
 * 
 * Now we entering the second major version of YYCC, I decide restore its original name and put it with other homebrew features.
*/
namespace yycc::carton::ironpad {

    /**
     * @brief The path info passed into user callback.
     * @details
     * For all pathes stored in this struct, if it is \c std::nullopt,
     * it means that handler fail to create this, otherwise it must be created.
     */
    struct CallbackInfo {
        std::optional<std::u8string> log_path;      ///< The path to crash log file.
        std::optional<std::u8string> coredump_path; ///< The path to coredump file.
    };

    /**
	 * @brief The callback function prototype which will be called when unhandled exception happened.
	 * @details
	 * During registering unhandled exception handler,
	 * caller can optionally provide a function pointer matching this prorotype to register.
	 * Then it will be called if unhandled exception hanppened.
     * The timing of calling this callback is the end of writing all essential files and before exiting handler.
     * In other words, all passed pathes are valid for visiting if they are not \c std::nullopt.
	 * 
	 * This callback is convenient for programmer using an explicit way to tell user an exception happened.
	 * Because in default, handler will only write error log to \c stderr and file.
	 * It will be totally invisible in GUI application.
	*/
    using ExceptionCallback = void (*)(const CallbackInfo& info);

    /**
	 * @brief Register unhandled exception handler
	 * @details
	 * This function will set an internal function as unhandled exception handler.
	 * 
	 * When unhandled exception raised, 
	 * That internal function will output error stacktrace in standard output,
	 * and generate log file and dump file in \c \%LOCALAPPDATA\%/IronPad folder if it is possible.
	 * (for convenient debugging of developer when reporting bugs.)
	 * 
	 * This function usually is called at the start of program.
	 * @param[in] callback User defined callback called when unhandled exception happened. nullptr if no callback.
     * @return True when success, otherwise false.
	*/
    bool startup(ExceptionCallback callback = nullptr);

    /**
	 * @brief Unregister unhandled exception handler
	 * @details 
	 * The reverse operation of startup().
	 * 
	 * This function and startup() should always be used as a pair.
	 * You must call this function to release reources if you have called startup().
	 * 
	 * This function usually is called at the end of program.
     * 
     * It is safe that call this function multiple times, or call this function when startup() return false.
     * It means that there is no compulsory check for the return value of startup() if you don't care it.
	*/
    void shutdown();

} // namespace yycc::carton::ironpad
