#pragma once
#include "YYCCInternal.hpp"

#include <cstdio>
#include <string>

/**
 * @brief The helper providing universal C\# style console function and other console related stuff
 * @details
 * For how to utilize this functions provided by this namespace, please view \ref console_helper.
*/
namespace YYCC::ConsoleHelper {

	/**
	 * @brief Enable console color support for Windows.
	 * @details This actually is enable virtual console feature for \c stdout and \c stderr.
	 * @return True if success, otherwise false.
	 * @remarks 
	 * This function only works on Windows and do nothing on other platforms such as Linux,
	 * because we assume all terminals existing on other platform support color feature as default.
	*/
	bool EnableColorfulConsole();

	/**
	 * @brief Reads the next line of UTF8 characters from the standard input stream.
	 * @return
	 * The next line of UTF8 characters from the input stream.
	 * Empty string if user just press Enter key or function failed.
	*/
	yycc_u8string ReadLine();

	/**
	 * @brief 
	 * Writes the text representation of the specified object 
	 * to the standard output stream using the specified format information.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments of format string.
	*/
	void Format(const yycc_char8_t* u8_fmt, ...);
	/**
	 * @brief 
	 * Writes the text representation of the specified object, 
	 * followed by the current line terminator, 
	 * to the standard output stream using the specified format information.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments of format string.
	*/
	void FormatLine(const yycc_char8_t* u8_fmt, ...);
	/**
	 * @brief Writes the specified string value to the standard output stream.
	 * @param[in] u8_strl The value to write.
	*/
	void Write(const yycc_char8_t* u8_strl);
	/**
	 * @brief 
	 * Writes the specified string value, followed by the current line terminator, 
	 * to the standard output stream.
	 * @param[in] u8_strl The value to write.
	*/
	void WriteLine(const yycc_char8_t* u8_strl);

	/**
	 * @brief 
	 * Writes the text representation of the specified object 
	 * to the standard error stream using the specified format information.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments of format string.
	*/
	void ErrFormat(const yycc_char8_t* u8_fmt, ...);
	/**
	 * @brief 
	 * Writes the text representation of the specified object, 
	 * followed by the current line terminator, 
	 * to the standard error stream using the specified format information.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments of format string.
	*/
	void ErrFormatLine(const yycc_char8_t* u8_fmt, ...);
	/**
	 * @brief Writes the specified string value to the standard error stream.
	 * @param[in] u8_strl The value to write.
	*/
	void ErrWrite(const yycc_char8_t* u8_strl);
	/**
	 * @brief 
	 * Writes the specified string value, followed by the current line terminator, 
	 * to the standard error stream.
	 * @param[in] u8_strl The value to write.
	*/
	void ErrWriteLine(const yycc_char8_t* u8_strl);

}
