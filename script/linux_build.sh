#!/bin/bash
README_PATH=$(pwd)/README.md
if [ ! -f "$README_PATH" ]; then
    echo "Error: You must run this script at the root folder of this project!"
    exit
fi

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
cmake -DCMAKE_BUILD_TYPE=Debug ../.. --fresh
cmake --build .
cmake --install . --prefix ../install/Debug
cmake -DCMAKE_BUILD_TYPE=Release -DYYCC_BUILD_TESTBENCH=ON ../.. --fresh
cmake --build .
cmake --install . --prefix ../install/Release
cd ..

# Exit to original path
cd ..
echo "Linux CMake Build Done"
