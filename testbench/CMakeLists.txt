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

# Install testbench only on Release mode
install(TARGETS YYCCTestbench
    CONFIGURATIONS Release RelWithDebInfo MinSizeRel
    RUNTIME DESTINATION ${YYCC_INSTALL_BIN_PATH}
)
