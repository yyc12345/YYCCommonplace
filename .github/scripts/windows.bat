@ECHO OFF

:: Create build directory and enter it
MKDIR bin
CD bin
:: Create internal build and install directory, then enter it
MKDIR build
MKDIR install

:: Build with x64 architecture in Release mode
CD build
cmake -A x64 -DYYCC_BUILD_TEST=ON -DGTest_ROOT=%GTest_ROOT% -DYYCC_BUILD_BENCHMARK=ON -Dbenchmark_ROOT=%benchmark_ROOT% ../..
cmake --build . --config Release
cmake --install . --prefix=../install --config Release
CD ..

:: Back to root directory
CD ..
