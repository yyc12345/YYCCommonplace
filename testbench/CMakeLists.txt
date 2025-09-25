# Create executable testbench
add_executable(YYCCTestbench "")
# Setup testbench sources
target_sources(YYCCTestbench
PRIVATE
    main.cpp
    
    shared/literals.cpp

    yycc/macro/version_cmp.cpp
    yycc/macro/os_detector.cpp
    yycc/macro/compiler_detector.cpp
    yycc/macro/endian_detector.cpp
    yycc/macro/ptr_size_detector.cpp
    yycc/macro/stl_detector.cpp
    yycc/flag_enum.cpp
    yycc/constraint.cpp
    yycc/constraint/builder.cpp
    yycc/patch/ptr_pad.cpp
    yycc/patch/fopen.cpp
    yycc/patch/stream.cpp
    yycc/patch/format.cpp
    yycc/rust/env.cpp
    yycc/string/reinterpret.cpp
    yycc/string/op.cpp
    yycc/num/parse.cpp
    yycc/num/stringify.cpp
    yycc/num/op.cpp
    yycc/num/safe_cast.cpp
    yycc/num/safe_op.cpp
    yycc/encoding/stl.cpp
    yycc/encoding/windows.cpp
    yycc/encoding/iconv.cpp
    yycc/windows/com.cpp
    yycc/windows/dialog.cpp
    yycc/windows/winfct.cpp
    yycc/windows/console.cpp
    
    yycc/carton/pycodec.cpp
    yycc/carton/termcolor.cpp
    yycc/carton/wcwidth.cpp
    yycc/carton/tabulate.cpp
    yycc/carton/clap.cpp
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
