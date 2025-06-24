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
    yycc/string/parse.cpp
    yycc/string/stringify.cpp
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
# Setup C++ standard
target_compile_features(YYCCTestbench PUBLIC cxx_std_17)
set_target_properties(YYCCTestbench PROPERTIES CXX_EXTENSION OFF)
# Order Unicode charset for private using
target_compile_definitions(YYCCTestbench
PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:UNICODE>
    $<$<CXX_COMPILER_ID:MSVC>:_UNICODE>
)
# Order build as UTF-8 in MSVC
target_compile_options(YYCCTestbench
PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
)

# Discover all test
include(GoogleTest)
gtest_discover_tests(YYCCTestbench)
