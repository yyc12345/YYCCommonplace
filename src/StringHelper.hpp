#pragma once
#include "YYCCInternal.hpp"

#include <string>
#include <cstdarg>
#include <functional>
#include <vector>

namespace YYCC::StringHelper {

	bool Printf(yycc_u8string& strl, const yycc_char8_t* format, ...);
	bool VPrintf(yycc_u8string& strl, const yycc_char8_t* format, va_list argptr);
	yycc_u8string Printf(const yycc_char8_t* format, ...);
	yycc_u8string VPrintf(const yycc_char8_t* format, va_list argptr);

	void Replace(yycc_u8string& strl, const yycc_char8_t* _from_strl, const yycc_char8_t* _to_strl);
	yycc_u8string Replace(const yycc_char8_t* _strl, const yycc_char8_t* _from_strl, const yycc_char8_t* _to_strl);

	/**
	 * @brief The data provider of general Join function.
	 * For the implementation of this function:
	 * Function return true to continue join. otherwise return false to terminate join. 
	 * The argument assigned in the calling returning false is not included.
	 * During calling, implementation should assign the string view to the string need to be joined in given argument.
	*/
	using JoinDataProvider = std::function<bool(yycc_u8string_view&)>;
	/**
	 * @brief General Join function.
	 * @details This function use function pointer as a general data provider interface,
	 * so this function suit for all types container, the user only need write a little bit adaption code.
	 * @param fct_data[in] The function pointer to data provider.
	 * @param decilmer[in] The decilmer.
	 * @return A std::string instance which containing the join result.
	*/
	yycc_u8string Join(JoinDataProvider fct_data, const yycc_char8_t* decilmer);
	/**
	 * @brief Specialized Join function for common used container.
	 * @param data
	 * @param decilmer
	 * @param reversed
	 * @return
	*/
	yycc_u8string Join(const std::vector<yycc_u8string>& data, const yycc_char8_t* decilmer, bool reversed = false);

	yycc_u8string Lower(const yycc_char8_t* strl);
	void Lower(yycc_u8string& strl);
	yycc_u8string Upper(const yycc_char8_t* strl);
	void Upper(yycc_u8string& strl);

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
	std::vector<yycc_u8string> Split(const yycc_char8_t* _strl, const yycc_char8_t* _decilmer);
}
