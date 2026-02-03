@ECHO OFF

:: Create build and install directory
MKDIR build
MKDIR install

:: Build project
CD build
cmake -A x64 -DCMAKE_CXX_STANDARD=23 -Dgtest_force_shared_crt=ON  ..
cmake --build . --config Release
cmake --install . --prefix=../install --config Release
CD ..

:: Record install directory
CD install
SET GTest_ROOT=%CD%
CD ..
