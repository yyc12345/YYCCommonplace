cmake_minimum_required(VERSION 3.23)
project(YYCC
    VERSION 2.0.0
    LANGUAGES CXX
)

# Setup C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Provide options
option(YYCC_BUILD_TEST "Build test of YYCCommonplace." OFF)
option(YYCC_BUILD_BENCHMARK "Build benchmark of YYCCommonplace." OFF)
option(YYCC_BUILD_DOC "Build document of YYCCommonplace." OFF)
option(YYCC_ENFORCE_ICONV "Enforce iconv support for this library (e.g. in MSYS2 environment)." OFF)

# Setup install path from CMake provided install path for convenient use.
include(GNUInstallDirs)
set(YYCC_INSTALL_INCLUDE_PATH ${CMAKE_INSTALL_INCLUDEDIR} CACHE PATH
"Public header install path relative to CMAKE_INSTALL_PREFIX unless set to an absolute path.")
set(YYCC_INSTALL_LIB_PATH ${CMAKE_INSTALL_LIBDIR} CACHE PATH
"Library install path relative to CMAKE_INSTALL_PREFIX unless set to an absolute path.")
set(YYCC_INSTALL_BIN_PATH ${CMAKE_INSTALL_BINDIR} CACHE PATH
"Binary install path relative to CMAKE_INSTALL_PREFIX unless set to an absolute path.")
set(YYCC_INSTALL_DOC_PATH ${CMAKE_INSTALL_DOCDIR} CACHE PATH
"Non-arch doc install path relative to CMAKE_INSTALL_PREFIX unless set to an absolute path.")

# Include dependency.
# GTest is required if we build test
if (YYCC_BUILD_TEST)
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    find_package(GTest REQUIRED)
endif ()
# Google Benchmark is required if we build benchmark
if (YYCC_BUILD_BENCHMARK)
    find_package(benchmark REQUIRED)
endif ()
# Iconv is required if we are not in Windows or user request it
if (YYCC_ENFORCE_ICONV OR (NOT WIN32))
    find_package(Iconv REQUIRED)
endif ()

# Import 4 build targets
add_subdirectory(src)
if (YYCC_BUILD_TEST)
    add_subdirectory(test)
endif ()
if (YYCC_BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif ()
if (YYCC_BUILD_DOC)
    add_subdirectory(doc)
endif ()

# Install target and package
# Install target
install(EXPORT YYCCommonplaceTargets
    FILE YYCCommonplaceTargets.cmake
    NAMESPACE YYCC::
    DESTINATION ${YYCC_INSTALL_LIB_PATH}/cmake/YYCCommonplace
)
# Package configuration file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    YYCCommonplaceConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMinorVersion
)
configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/YYCCommonplaceConfig.cmake.in 
    "${CMAKE_CURRENT_BINARY_DIR}/YYCCommonplaceConfig.cmake"
    INSTALL_DESTINATION ${YYCC_INSTALL_LIB_PATH}/cmake/YYCCommonplace
)
# Copy package files to install destination
install(
FILES
    "${CMAKE_CURRENT_BINARY_DIR}/YYCCommonplaceConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/YYCCommonplaceConfigVersion.cmake"
DESTINATION
    ${YYCC_INSTALL_LIB_PATH}/cmake/YYCCommonplace
)

