cmake_minimum_required(VERSION 3.23)
project(YYCC
    VERSION 1.1.0
    LANGUAGES CXX
)

# Provide options
option(YYCC_BUILD_TESTBENCH "Build testbench of YYCCommonplace." OFF)
option(YYCC_BUILD_DOC "Build document of YYCCommonplace." OFF)

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

# Import 3 build targets
add_subdirectory(src)
if (YYCC_BUILD_TESTBENCH)
    add_subdirectory(testbench)
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
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/YYCCommonplace
)
# Copy package files to install destination
install(
FILES
    "${CMAKE_CURRENT_BINARY_DIR}/YYCCommonplaceConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/YYCCommonplaceConfigVersion.cmake"
DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/YYCCommonplace
)

