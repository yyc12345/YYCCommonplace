# Create executable testbench
add_executable(YYCCTestbench "")
# Setup testbench sources
target_sources(YYCCTestbench
PRIVATE
    shared/literals.cpp

    main.cpp
    yycc/macro/version_cmp.cpp
    yycc/flag_enum.cpp
    yycc/constraint.cpp
    yycc/constraint/builder.cpp
    yycc/patch/ptr_pad.cpp
    yycc/string/op.cpp
    yycc/string/reinterpret.cpp
    yycc/num/parse.cpp
    yycc/num/stringify.cpp
    yycc/num/op.cpp
    yycc/num/safe_cast.cpp
    yycc/num/safe_op.cpp
    yycc/encoding/stl.cpp
    yycc/encoding/windows.cpp
    yycc/encoding/iconv.cpp
    
    yycc/carton/pycodec.cpp
)
target_sources(YYCCTestbench
PRIVATE
FILE_SET HEADERS
FILES
    shared/literals.hpp
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
