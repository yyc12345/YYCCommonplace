#!/bin/bash
set -euo pipefail

# Create build and install directory
mkdir build install

# Build project
cd build
cmake -DCMAKE_CXX_STANDARD=23 -Dgtest_force_shared_crt=ON -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
cmake --install . --prefix=../install
cd ..

# Record install directory
cd install
export GTest_ROOT=$(pwd)
cd ..
