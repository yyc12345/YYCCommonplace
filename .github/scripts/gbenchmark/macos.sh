#!/bin/bash
set -euo pipefail

# Create build and install directory
mkdir build install

# Build project
cd build
cmake -DCMAKE_CXX_STANDARD=23 -DBENCHMARK_ENABLE_TESTING=OFF -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
cmake --install . --prefix=../install
cd ..

# Record install directory
cd install
export benchmark_ROOT=$(pwd)
cd ..
