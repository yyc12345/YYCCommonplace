# Create static library
add_library(YYCCommonplace STATIC "")
# Setup static library sources
target_sources(YYCCommonplace
PRIVATE
    # Headers
    # Common headers
    ${CMAKE_CURRENT_LIST_DIR}/ConsoleHelper.hpp
    ${CMAKE_CURRENT_LIST_DIR}/DialogHelper.hpp
    ${CMAKE_CURRENT_LIST_DIR}/EncodingHelper.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ExceptionHelper.hpp
    ${CMAKE_CURRENT_LIST_DIR}/IOHelper.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ParserHelper.hpp
    ${CMAKE_CURRENT_LIST_DIR}/StringHelper.hpp
    # Windows including guard pair
    ${CMAKE_CURRENT_LIST_DIR}/WinImportPrefix.hpp
    ${CMAKE_CURRENT_LIST_DIR}/WinImportSuffix.hpp
    # Misc
    ${CMAKE_CURRENT_LIST_DIR}/YYCCInternal.hpp
    ${CMAKE_CURRENT_LIST_DIR}/YYCCommonplace.hpp

    # Sources
    ${CMAKE_CURRENT_LIST_DIR}/ConsoleHelper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DialogHelper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/EncodingHelper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ExceptionHelper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/IOHelper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ParserHelper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/StringHelper.cpp
)
# Setup header infomations
target_include_directories(YYCCommonplace
PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
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

# Install project
# Install binary
install(TARGETS YYCCommonplace
    EXPORT YYCCommonplaceTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
# Install headers
install(
FILES
    # Common headers
    ${CMAKE_CURRENT_LIST_DIR}/ConsoleHelper.hpp
    ${CMAKE_CURRENT_LIST_DIR}/DialogHelper.hpp
    ${CMAKE_CURRENT_LIST_DIR}/EncodingHelper.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ExceptionHelper.hpp
    ${CMAKE_CURRENT_LIST_DIR}/IOHelper.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ParserHelper.hpp
    ${CMAKE_CURRENT_LIST_DIR}/StringHelper.hpp
    # Windows including guard pair
    ${CMAKE_CURRENT_LIST_DIR}/WinImportPrefix.hpp
    ${CMAKE_CURRENT_LIST_DIR}/WinImportSuffix.hpp
    # Misc
    ${CMAKE_CURRENT_LIST_DIR}/YYCCInternal.hpp
    ${CMAKE_CURRENT_LIST_DIR}/YYCCommonplace.hpp
DESTINATION include
)
