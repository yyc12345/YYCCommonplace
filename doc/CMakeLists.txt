# Configure Doxygen config file
configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    @ONLY
)
