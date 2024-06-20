# Create static library
add_library(YYCCommonplace STATIC "")
# Setup static library sources
target_sources(YYCCommonplace
PRIVATE
    # Sources
    COMHelper.cpp
    ConsoleHelper.cpp
    DialogHelper.cpp
    EncodingHelper.cpp
    ExceptionHelper.cpp
    FsPathPatch.cpp
    IOHelper.cpp
    StringHelper.cpp
    WinFctHelper.cpp
)
target_sources(YYCCommonplace 
PUBLIC
FILE_SET HEADERS
FILES
    # Headers
    # Common headers
    COMHelper.hpp
    ConsoleHelper.hpp
    DialogHelper.hpp
    EncodingHelper.hpp
    ExceptionHelper.hpp
    FsPathPatch.hpp
    IOHelper.hpp
    ParserHelper.hpp
    StringHelper.hpp
    WinFctHelper.hpp
    # Windows including guard pair
    WinImportPrefix.hpp
    WinImportSuffix.hpp
    # Misc
    YYCCInternal.hpp
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
set_target_properties(YYCCommonplace
PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED 17
    CXX_EXTENSION OFF
)
# Order Unicode charset for private using
target_compile_definitions(YYCCommonplace
PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:UNICODE>
    $<$<CXX_COMPILER_ID:MSVC>:_UNICODE>
)
# Order build as UTF-8 in MSVC
target_compile_options(YYCCommonplace
PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
)

# Install binary and headers
install(TARGETS YYCCommonplace
    EXPORT YYCCommonplaceTargets
    LIBRARY DESTINATION ${YYCC_INSTALL_PATH_LIB}
    ARCHIVE DESTINATION ${YYCC_INSTALL_PATH_LIB}
    INCLUDES DESTINATION include
    FILE_SET HEADERS DESTINATION include
)
