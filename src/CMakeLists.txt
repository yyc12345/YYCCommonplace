# Setup file lists up
set(YYCC_HEADER ${CMAKE_CURRENT_LIST_DIR})
set(YYCC_SRC
    DialogHelper.cpp
    EncodingHelper.cpp
    ExceptionHelper.cpp
    IOHelper.cpp
    ParserHelper.cpp
    StringHelper.cpp
    TerminalHelper.cpp
)

# Create static library
add_library(YYCCommonplace
    ${YYCC_SRC}
)
# Setup header infomations
target_include_directories(YYCCommonplace
PUBLIC
    ${YYCC_HEADER}
)
# Setup C++ standard
set_target_properties(YYCCommonplace
PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED 17
    CXX_EXTENSION OFF
)
# Disable MSVC standard library warnings
target_compile_definitions(YYCCommonplace
PUBLIC
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_NONSTDC_NO_DEPRECATE>
)
# Order build as UTF-8 in MSVC
target_compile_options(YYCCommonplace
PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
)
