#!/bin/bash
# Navigate to project root directory
cd {{ repo_root_dir }}

# Create main binary directory
mkdir bin
cd bin
# Create build directory
mkdir build
# Create install directory
mkdir install
cd install
mkdir Debug
mkdir Release
cd ..

# Build current system debug and release version
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_STANDARD={{ cpp_version }} {{ '-DCMAKE_POSITION_INDEPENDENT_CODE=True' if pic }} ../.. --fresh
cmake --build .
cmake --install . --prefix ../install/Debug
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_CXX_STANDARD={{ cpp_version }} {{ '-DCMAKE_POSITION_INDEPENDENT_CODE=True' if pic }} -DYYCC_BUILD_TESTBENCH=ON ../.. --fresh
cmake --build .
cmake --install . --prefix ../install/Release
cd ..

# Exit to original path
cd ..
echo "Linux CMake Build Done"
