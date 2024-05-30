# Create executable testbench
add_executable(Testbench
    main.cpp
)
# Add YYCC as its library
target_include_directories(Testbench
PRIVATE
    YYCCommonplace
)
target_link_libraries(Testbench
PRIVATE
    YYCCommonplace
)
# Setup C++ standard
set_target_properties(Testbench
PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED 17
    CXX_EXTENSION OFF
)
# Order build as UTF-8 in MSVC
target_compile_options(Testbench
PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
)
