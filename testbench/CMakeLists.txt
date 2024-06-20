# Create executable testbench
add_executable(YYCCTestbench "")
# Setup testbench sources
target_sources(YYCCTestbench
PRIVATE
    main.cpp
)
# Add YYCC as its library
target_include_directories(YYCCTestbench
PRIVATE
    YYCCommonplace
)
target_link_libraries(YYCCTestbench
PRIVATE
    YYCCommonplace
)
# Setup C++ standard
set_target_properties(YYCCTestbench
PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED 17
    CXX_EXTENSION OFF
)
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

# Install testbench only on Release mode
install(TARGETS YYCCTestbench
    EXPORT YYCCTestbenchTargets
    CONFIGURATIONS Release
    RUNTIME DESTINATION ${YYCC_INSTALL_PATH_BIN}
)
