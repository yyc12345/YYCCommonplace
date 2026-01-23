message(STATUS "Checking charconv implementation...")
include(CheckCXXSourceCompiles)

file(READ "${CMAKE_CURRENT_LIST_DIR}/check_charconv/chars_format.cpp" TEST_CODE_SNIPPET)
check_cxx_source_compiles("${TEST_CODE_SNIPPET}" YYCC_CHARCONV_HAS_CHARS_FORMAT)
message(STATUS "Support std::chars_format: ${YYCC_CHARCONV_HAS_CHARS_FORMAT}")

file(READ "${CMAKE_CURRENT_LIST_DIR}/check_charconv/from_chars_result.cpp" TEST_CODE_SNIPPET)
check_cxx_source_compiles("${TEST_CODE_SNIPPET}" YYCC_CHARCONV_HAS_FROM_CHARS_RESULT)
message(STATUS "Support std::from_chars_result: ${YYCC_CHARCONV_HAS_FROM_CHARS_RESULT}")

file(READ "${CMAKE_CURRENT_LIST_DIR}/check_charconv/to_chars_result.cpp" TEST_CODE_SNIPPET)
check_cxx_source_compiles("${TEST_CODE_SNIPPET}" YYCC_CHARCONV_HAS_TO_CHARS_RESULT)
message(STATUS "Support std::to_chars_result: ${YYCC_CHARCONV_HAS_TO_CHARS_RESULT}")

file(READ "${CMAKE_CURRENT_LIST_DIR}/check_charconv/from_chars_int.cpp" TEST_CODE_SNIPPET)
check_cxx_source_compiles("${TEST_CODE_SNIPPET}" YYCC_CHARCONV_HAS_FROM_CHARS_INT)
message(STATUS "Support std::from_chars with integral type: ${YYCC_CHARCONV_HAS_FROM_CHARS_INT}")

file(READ "${CMAKE_CURRENT_LIST_DIR}/check_charconv/from_chars_float.cpp" TEST_CODE_SNIPPET)
check_cxx_source_compiles("${TEST_CODE_SNIPPET}" YYCC_CHARCONV_HAS_FROM_CHARS_FLOAT)
message(STATUS "Suppoer std::from_chars with float point type: ${YYCC_CHARCONV_HAS_FROM_CHARS_FLOAT}")

file(READ "${CMAKE_CURRENT_LIST_DIR}/check_charconv/to_chars_int.cpp" TEST_CODE_SNIPPET)
check_cxx_source_compiles("${TEST_CODE_SNIPPET}" YYCC_CHARCONV_HAS_TO_CHARS_INT)
message(STATUS "Support std::to_chars with integral type: ${YYCC_CHARCONV_HAS_TO_CHARS_INT}")

file(READ "${CMAKE_CURRENT_LIST_DIR}/check_charconv/to_chars_float.cpp" TEST_CODE_SNIPPET)
check_cxx_source_compiles("${TEST_CODE_SNIPPET}" YYCC_CHARCONV_HAS_TO_CHARS_FLOAT)
message(STATUS "Support std::to_chars with float point type: ${YYCC_CHARCONV_HAS_TO_CHARS_FLOAT}")
