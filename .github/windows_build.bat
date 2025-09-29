@ECHO OFF

:: Create build directory and enter it
MKDIR bin
CD bin
:: Create internal build and install directory, then enter it
MKDIR build
MKDIR install
CD build

:: Build with x64 architecture in Release mode
cmake -A x64 ../..
cmake --build . --config Release
cmake --install . --prefix=../install --config Relese

:: Back to root directory
CD ..
CD ..
