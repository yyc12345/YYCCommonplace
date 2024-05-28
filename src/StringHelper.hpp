#pragma once
#include "YYCCInternal.hpp"

#include <string>
#include <cstdarg>
#include <functional>
#include <vector>

namespace YYCC::StringHelper {
	
	bool Printf(std::string& strl, const char* format, ...);
	bool VPrintf(std::string& strl, const char* format, va_list argptr);

	std::string Printf(const char* format, ...);
	std::string VPrintf(const char* format, va_list argptr);

	/**
	 * @brief The data provider of general Join function.
	 * This function pointer return non-null string pointer to represent a element of joined series.
	 * otherwise return nullptr to terminate the joining process.
	*/
	using JoinDataProvider = std::function<const char*()>;
	/**
	 * @brief General Join function.
	 * @details This function use function pointer as a general data provider interface,
	 * so this function suit for all types container, the user only need write a little bit adaption code.
	 * @param fct_data[in] The function pointer to data provider.
	 * @param decilmer[in] The decilmer.
	 * @return A std::string instance which containing the join result.
	*/
	std::string Join(JoinDataProvider fct_data, const char* decilmer);
	/**
	 * @brief Specialized Join function for common used container.
	 * @param data 
	 * @param decilmer 
	 * @param reversed 
	 * @return 
	*/
	std::string Join(const std::vector<std::string>& data, const char* decilmer, bool reversed = false);

	/**
	 * @brief Transform string to lower.
	 * @param strl 
	 * @return 
	*/
	std::string Lower(const char* strl);
	void Lower(std::string& strl);
	/**
	 * @brief Transform string to upper.
	 * @param strl 
	 * @return 
	*/
	std::string Upper(const char* strl);
	void Upper(std::string& strl);

	/**
	 * @brief General Split function.
	 * @param _strl[in] The string need to be splitting.
	 * If this is nullptr, the result will be empty.
	 * @param _decilmer[in] The decilmer for splitting.
	 * If decilmer is nullptr or zero length, the result will only have 1 element which is original string.
	 * @return 
	 * @remarks This function may be low performance because it just a homebrew Split functon.
	 * It can works in most toy cases but not suit for high performance scenario.
	 * Also, this function will produce a copy of original string because it is not zero copy.
	*/
	std::vector<std::string> Split(const char* _strl, const char* _decilmer);
}
