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
    yycc/rust/panic.cpp
    yycc/patch/path.cpp
    yycc/encoding/utf.cpp
    yycc/encoding/windows.cpp
    yycc/encoding/iconv.cpp
    yycc/encoding/pycodec.cpp
    # YYCC/COMHelper.cpp
    # YYCC/ArgParser.cpp
    # YYCC/ConfigManager.cpp
    # YYCC/ConsoleHelper.cpp
    # YYCC/DialogHelper.cpp
    # YYCC/EncodingHelper.cpp
    # YYCC/ExceptionHelper.cpp
    # YYCC/StdPatch.cpp
    # YYCC/IOHelper.cpp
    # YYCC/StringHelper.cpp
    # YYCC/WinFctHelper.cpp
    # # Natvis (only for MSVC)
    # $<$<CXX_COMPILER_ID:MSVC>:YYCC.natvis>
)
target_sources(YYCCommonplace 
PUBLIC
FILE_SET HEADERS
FILES
    # Headers
    yycc.hpp
    yycc/version.hpp
    yycc/prelude/core.hpp
    yycc/prelude/rust.hpp
    yycc/macro/version_cmp.hpp
    yycc/macro/feature_probe.hpp
    yycc/macro/os_detector.hpp
    yycc/macro/class_copy_move.hpp
    yycc/string.hpp
    yycc/string/reinterpret.hpp
    yycc/string/op.hpp
    yycc/num/parse.hpp
    yycc/num/stringify.hpp
    yycc/rust/primitive.hpp
    yycc/rust/panic.hpp
    yycc/rust/option.hpp
    yycc/rust/result.hpp
    yycc/rust/num/parse.hpp
    yycc/rust/num/stringify.hpp
    yycc/windows/unsafe_suppressor.hpp
    yycc/windows/import_guard_head.hpp
    yycc/windows/import_guard_tail.hpp
    yycc/constraint.hpp
    yycc/constraint/builder.hpp
    yycc/patch/path.hpp
    yycc/patch/contains.hpp
    yycc/patch/starts_ends_with.hpp
    yycc/patch/expected.hpp
    yycc/encoding/utf.hpp
    yycc/encoding/windows.hpp
    yycc/encoding/iconv.hpp
    yycc/encoding/pycodec.hpp

    # # Headers
    # # Common headers
    # YYCC/Constraints.hpp
    # YYCC/COMHelper.hpp
    # YYCC/ArgParser.hpp
    # YYCC/ConfigManager.hpp
    # YYCC/ConsoleHelper.hpp
    # YYCC/DialogHelper.hpp
    # YYCC/EncodingHelper.hpp
    # YYCC/EnumHelper.hpp
    # YYCC/ExceptionHelper.hpp
    # YYCC/StdPatch.hpp
    # YYCC/IOHelper.hpp
    # YYCC/ParserHelper.hpp
    # YYCC/StringHelper.hpp
    # YYCC/WinFctHelper.hpp
    # # Windows including guard pair
    # YYCC/WinImportPrefix.hpp
    # YYCC/WinImportSuffix.hpp
    # # Internal
    # YYCC/YYCCVersion.hpp
    # YYCC/YYCCInternal.hpp
    # # Exposed
    # YYCCommonplace.hpp
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
# Setup C++ standard
target_compile_features(YYCCommonplace PUBLIC cxx_std_17)
set_target_properties(YYCCommonplace PROPERTIES CXX_EXTENSION OFF)
# Setup macros
target_compile_definitions(YYCCommonplace
PUBLIC
    # Debug macro. And it should populate to child projects
    $<$<BOOL:${YYCC_DEBUG_UE_FILTER}>:YYCC_DEBUG_UE_FILTER>
    # Iconv environment macro
    $<$<BOOL:${YYCC_ENFORCE_ICONV}>:YYCC_FEAT_ICONV>
PRIVATE
    # Unicode charset for private using
    $<$<CXX_COMPILER_ID:MSVC>:UNICODE>
    $<$<CXX_COMPILER_ID:MSVC>:_UNICODE>
)
target_compile_options(YYCCommonplace
# Order build as UTF-8 in MSVC
PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
)

# Install binary and headers
install(TARGETS YYCCommonplace
    EXPORT YYCCommonplaceTargets
    LIBRARY DESTINATION ${YYCC_INSTALL_LIB_PATH}
    ARCHIVE DESTINATION ${YYCC_INSTALL_LIB_PATH}
    INCLUDES DESTINATION ${YYCC_INSTALL_INCLUDE_PATH}
    FILE_SET HEADERS DESTINATION ${YYCC_INSTALL_INCLUDE_PATH}
)
