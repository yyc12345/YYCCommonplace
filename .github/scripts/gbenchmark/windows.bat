@ECHO OFF

:: Create build and install directory
MKDIR build
MKDIR install

:: Build project
CD build
cmake -A x64 -DCMAKE_CXX_STANDARD=23 -DBENCHMARK_ENABLE_TESTING=OFF ..
cmake --build . --config Release
cmake --install . --prefix=../install --config Release
CD ..

:: Record install directory
CD install
SET benchmark_ROOT=%CD%
CD ..
