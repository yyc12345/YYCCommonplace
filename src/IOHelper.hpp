#pragma once
#include "YYCCInternal.hpp"

#include <cstdio>
#include <filesystem>

namespace YYCC::IOHelper {
	
#if UINTPTR_MAX == UINT32_MAX
#define PRI_XPTR_LEFT_PADDING "08"
#elif UINTPTR_MAX == UINT64_MAX
	/**
	 * @brief The left-padding zero format string of HEX-printed pointer type.
	 * @details
	 * When printing a pointer with HEX style, we always hope it can be left-padded with some zero for easy reading.
	 * In different architecture, the size of this padding is differnet too so we create this macro.
	 * \n
	 * In 32-bit environment, it will be "08" meaning left pad zero until 8 number position.
	 * In 64-bit environment, it will be "016" meaning left pad zero until 16 number position.
	*/
#define PRI_XPTR_LEFT_PADDING "016"
#else
#error "Not supported pointer size."
#endif

	/**
	 * @brief The UTF8 version of std::fopen.
	 * @param u8_filepath[in] The UTF8 encoded path to the file to be opened.
	 * @param u8_mode[in] UTF8 encoded mode string of the file to be opened.
	 * @remarks
	 * This function is suit for Windows because std::fopen do not support UTF8 on Windows.
	 * On other platforms, this function will delegate request directly to std::fopen.
	 * @return FILE* of the file to be opened, or nullptr if failed.
	*/
	FILE* UTF8FOpen(const char* u8_filepath, const char* u8_mode);
	
	/**
	 * @brief Build std::filesystem::path from UTF8 string.
	 * @param u8_path[in] UTF8 path string for building this std::filesystem::path.
	 * @return std::filesystem::path instance.
	 * @exception std::invalid_argument Fail to parse given UTF8 string (maybe invalid?).
	 * @remarks
	 * This function is suit for Windows.
	 * On other platforms, it will simply call the constructor of std::filesystem::path.
	*/
	std::filesystem::path UTF8Path(const char* u8_path);

}
