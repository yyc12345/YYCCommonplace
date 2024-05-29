cmake_minimum_required(VERSION 3.12)
project(YYCCommonplace LANGUAGES CXX)

# Add core static library in build
add_subdirectory(src)
# Add testbench code in build
add_subdirectory(testbench)

