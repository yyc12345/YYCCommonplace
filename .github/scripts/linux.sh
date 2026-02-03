#!/bin/bash
set -euo pipefail

# Create build directory and enter it
mkdir bin
cd bin
# Create internal build and install directory, then enter it
mkdir build
mkdir install

# Build in Release mode
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DYYCC_BUILD_TEST=ON -DGTest_ROOT=$GTest_ROOT -DYYCC_BUILD_BENCHMARK=ON -Dbenchmark_ROOT=$benchmark_ROOT ../..
cmake --build .
cmake --install . --prefix=../install
cd ..

# Back to root directory
cd ..
