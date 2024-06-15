cmake_minimum_required(VERSION 3.23)
project(YYCC
    VERSION 1.0.0
    LANGUAGES CXX
)

# Provide options
option(YYCC_BUILD_TESTBENCH "Build testbench of YYCCommonplace." OFF)
option(YYCC_BUILD_DOC "Build document of YYCCommonplace." OFF)

# Import 2 build targets
add_subdirectory(src)
if (YYCC_BUILD_TESTBENCH)
    add_subdirectory(testbench)
endif ()
if (YYCC_BUILD_DOC)
    add_subdirectory(doc)
endif ()

# Install project package infos
# Package target
install(EXPORT YYCCommonplaceTargets
    FILE YYCCommonplaceTargets.cmake
    NAMESPACE YYCC::
    DESTINATION lib/cmake/YYCCommonplace
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
    INSTALL_DESTINATION lib/cmake/YYCCommonplace
)
# Copy to install destination
install(
FILES
    "${CMAKE_CURRENT_BINARY_DIR}/YYCCommonplaceConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/YYCCommonplaceConfigVersion.cmake"
DESTINATION
    lib/cmake/YYCCommonplace
)

