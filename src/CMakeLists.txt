# Configure version file
configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/yycc/version.hpp.in
    ${CMAKE_CURRENT_LIST_DIR}/yycc/version.hpp
    @ONLY
)

# Create static library
add_library(YYCCommonplace STATIC "")
# Setup static library sources
target_sources(YYCCommonplace
PRIVATE
    # Sources
    yycc/string/reinterpret.cpp
    yycc/string/op.cpp
    yycc/patch/fopen.cpp
    yycc/rust/panic.cpp
    yycc/windows/com.cpp
    #yycc/windows/dialog.cpp
    yycc/windows/winfct.cpp
    yycc/encoding/stl.cpp
    yycc/encoding/windows.cpp
    yycc/encoding/iconv.cpp

    yycc/carton/pycodec.cpp
)
target_sources(YYCCommonplace 
PUBLIC
FILE_SET HEADERS
FILES
    # Headers
    yycc.hpp
    yycc/version.hpp
    yycc/macro/version_cmp.hpp
    yycc/macro/os_detector.hpp
    yycc/macro/stl_detector.hpp
    yycc/macro/endian_detector.hpp
    yycc/macro/compiler_detector.hpp
    yycc/macro/ptr_size_detector.hpp
    yycc/macro/class_copy_move.hpp
    yycc/flag_enum.hpp
    yycc/string/reinterpret.hpp
    yycc/string/op.hpp
    yycc/patch/ptr_pad.hpp
    yycc/patch/fopen.hpp
    yycc/num/parse.hpp
    yycc/num/stringify.hpp
    yycc/num/safe_cast.hpp
    yycc/num/safe_op.hpp
    yycc/num/op.hpp
    yycc/rust/prelude.hpp
    yycc/rust/primitive.hpp
    yycc/rust/panic.hpp
    yycc/rust/option.hpp
    yycc/rust/result.hpp
    yycc/windows/import_guard_head.hpp
    yycc/windows/import_guard_tail.hpp
    yycc/windows/com.hpp
    #yycc/windows/dialog.hpp
    yycc/windows/winfct.hpp
    yycc/constraint.hpp
    yycc/constraint/builder.hpp
    yycc/encoding/stl.hpp
    yycc/encoding/windows.hpp
    yycc/encoding/iconv.hpp

    yycc/carton/pycodec.hpp
)
# Setup header infomations
target_include_directories(YYCCommonplace
PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
# Link Iconv if we have import it
if (Iconv_FOUND)
    target_link_libraries(YYCCommonplace
        PRIVATE
            Iconv::Iconv
    )
endif ()
# Link with DbgHelp.lib on Windows
target_link_libraries(YYCCommonplace
PRIVATE
    $<$<BOOL:${WIN32}>:DbgHelp.lib>
)
# Setup macros
target_compile_definitions(YYCCommonplace
PUBLIC
    # Iconv environment macro
    $<$<BOOL:${Iconv_FOUND}>:YYCC_FEAT_ICONV>
    # OS macro
    $<$<PLATFORM_ID:Windows>:YYCC_OS_WINDOWS>
    $<$<PLATFORM_ID:Linux>:YYCC_OS_LINUX>
    $<$<PLATFORM_ID:Darwin>:YYCC_OS_MACOS>
    # Compiler macro
    $<$<CXX_COMPILER_ID:GNU>:YYCC_CC_GCC>
    $<$<CXX_COMPILER_ID:Clang>:YYCC_CC_CLANG>
    $<$<CXX_COMPILER_ID:MSVC>:YYCC_CC_MSVC>
    # Endian macro
    $<$<STREQUAL:${CMAKE_CXX_BYTE_ORDER},LITTLE_ENDIAN>:YYCC_ENDIAN_LITTLE>
    $<$<STREQUAL:${CMAKE_CXX_BYTE_ORDER},BIG_ENDIAN>:YYCC_ENDIAN_BIG>
    # Pointer size macro
    $<$<EQUAL:${CMAKE_SIZEOF_VOID_P},4>:YYCC_PTRSIZE_32>
    $<$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>:YYCC_PTRSIZE_64>
    # Use Unicode charset on MSVC
    $<$<CXX_COMPILER_ID:MSVC>:UNICODE>
    $<$<CXX_COMPILER_ID:MSVC>:_UNICODE>
    # Fix MSVC shit
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_DEPRECATE>
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_NONSTDC_NO_WARNINGS>
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_NONSTDC_NO_DEPRECATE>
    # Fix Windows header file shit
    $<$<BOOL:${WIN32}>:WIN32_LEAN_AND_MEAN>
    $<$<BOOL:${WIN32}>:NOMINMAX>
)
target_compile_options(YYCCommonplace
PUBLIC
    # Order build as UTF-8 in MSVC
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
    # Order preprocessor conformance mode (fix __VA_OPT__ error in MSVC)
    $<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>
    # Resolve MSVC __cplusplus macro value error.
    $<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>
)

# Fix GCC std::stacktrace link error
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 14)
        target_link_libraries(YYCCommonplace PRIVATE stdc++exp)
    else ()
        target_link_libraries(YYCCommonplace PRIVATE stdc++_libbacktrace)
    endif ()
endif ()

# Install binary and headers
install(TARGETS YYCCommonplace
    EXPORT YYCCommonplaceTargets
    LIBRARY DESTINATION ${YYCC_INSTALL_LIB_PATH}
    ARCHIVE DESTINATION ${YYCC_INSTALL_LIB_PATH}
    INCLUDES DESTINATION ${YYCC_INSTALL_INCLUDE_PATH}
    FILE_SET HEADERS DESTINATION ${YYCC_INSTALL_INCLUDE_PATH}
)
