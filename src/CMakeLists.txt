# Create static library
add_library(YYCCommonplace STATIC "")
# Setup static library sources
target_sources(YYCCommonplace
PRIVATE
    # Sources
    YYCC/COMHelper.cpp
    YYCC/ArgParser.cpp
    YYCC/ConfigManager.cpp
    YYCC/ConsoleHelper.cpp
    YYCC/DialogHelper.cpp
    YYCC/EncodingHelper.cpp
    YYCC/ExceptionHelper.cpp
    YYCC/StdPatch.cpp
    YYCC/IOHelper.cpp
    YYCC/StringHelper.cpp
    YYCC/WinFctHelper.cpp
    # Natvis (only for MSVC)
    $<$<CXX_COMPILER_ID:MSVC>:YYCC.natvis>
)
target_sources(YYCCommonplace 
PUBLIC
FILE_SET HEADERS
FILES
    # Headers
    # Common headers
    YYCC/Constraints.hpp
    YYCC/COMHelper.hpp
    YYCC/ArgParser.hpp
    YYCC/ConfigManager.hpp
    YYCC/ConsoleHelper.hpp
    YYCC/DialogHelper.hpp
    YYCC/EncodingHelper.hpp
    YYCC/EnumHelper.hpp
    YYCC/ExceptionHelper.hpp
    YYCC/StdPatch.hpp
    YYCC/IOHelper.hpp
    YYCC/ParserHelper.hpp
    YYCC/StringHelper.hpp
    YYCC/WinFctHelper.hpp
    # Windows including guard pair
    YYCC/WinImportPrefix.hpp
    YYCC/WinImportSuffix.hpp
    # Internal
    YYCC/YYCCInternal.hpp
    # Exposed
    YYCCommonplace.hpp
)
# Setup header infomations
target_include_directories(YYCCommonplace
PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
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
# Debug macro should populate to child projects
PUBLIC
    $<$<BOOL:${YYCC_DEBUG_UE_FILTER}>:YYCC_DEBUG_UE_FILTER>
# Unicode charset for private using
PRIVATE
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
