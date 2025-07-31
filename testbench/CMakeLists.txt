# Create executable testbench
add_executable(YYCCTestbench "")
# Setup testbench sources
target_sources(YYCCTestbench
PRIVATE
    main.cpp
    yycc/constraint.cpp
    yycc/constraint/builder.cpp
    yycc/string/op.cpp
    yycc/string/reinterpret.cpp
    yycc/num/parse.cpp
    yycc/num/stringify.cpp
)
target_sources(YYCCTestbench
PRIVATE
FILE_SET HEADERS
FILES
    yycc/encoding/utf_literal.hpp
)
# Setup headers
target_include_directories(YYCCTestbench
PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}"
)
# Setup libraries
target_link_libraries(YYCCTestbench
PRIVATE
    YYCCommonplace
    GTest::gtest_main
)

# Discover all test
include(GoogleTest)
gtest_discover_tests(YYCCTestbench)
