#pragma once
#include "YYCCInternal.hpp"

#include <cstdio>
#include <filesystem>

/**
 * @brief Some IO related stuff
 * @details
 * See also \ref io_helper.
*/
namespace YYCC::IOHelper {

	/// @brief C++ standard deleter for std::FILE*
	class StdFileDeleter {
	public:
		StdFileDeleter() {}
		void operator() (std::FILE* ptr) {
			if (ptr != nullptr) {
				std::fclose(ptr);
			}
		}
	};
	/// @brief Smart unique pointer of \c std::FILE*
	using SmartStdFile = std::unique_ptr<std::FILE, StdFileDeleter>;

	/**
	 * @brief The UTF8 version of \c std::fopen.
	 * @param[in] u8_filepath The UTF8 encoded path to the file to be opened.
	 * @param[in] u8_mode UTF8 encoded mode string of the file to be opened.
	 * @remarks
	 * This function is suit for Windows because std::fopen do not support UTF8 on Windows.
	 * On other platforms, this function will delegate request directly to std::fopen.
	 * @return \c FILE* of the file to be opened, or nullptr if failed.
	*/
	std::FILE* UTF8FOpen(const yycc_char8_t* u8_filepath, const yycc_char8_t* u8_mode);
	
}
