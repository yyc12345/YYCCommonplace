@ECHO OFF
SET README_PATH=%CD%\README.md
IF EXIST %README_PATH% (
    REM DO NOTHING
) ELSE (
    ECHO Error: You must run this script at the root folder of this project!
    EXIT /b
)

:: Create main binary directory
MKDIR bin
CD bin
:: Create build folder
MKDIR Win32
MKDIR x64
MKDIR documentation
:: Create install folder
MKDIR install
CD install
MKDIR Win32_Debug
MKDIR Win32_Release
MKDIR x64_Debug
MKDIR x64_Release
CD ..

:: Build for Win32
CD Win32
cmake -G "Visual Studio 16 2019" -A Win32 -DYYCC_BUILD_TESTBENCH=ON ../..
cmake --build . --config Debug
cmake --install . --prefix=../install/Win32_Debug --config Debug
cmake --build . --config Release
cmake --install . --prefix=../install/Win32_Release --config Release
CD ..

:: Build for x64
CD x64
cmake -G "Visual Studio 16 2019" -A x64 -DYYCC_BUILD_TESTBENCH=ON ../..
cmake --build . --config Debug
cmake --install . --prefix=../install/x64_Debug --config Debug
cmake --build . --config Release
cmake --install . --prefix=../install/x64_Release --config Release
CD ..

:: Build for documentation
CD documentation
cmake -G "Visual Studio 16 2019" -A x64 -DYYCC_BUILD_DOC=ON ../..
cmake --build . --config Release
cmake --build . --target YYCCDocumentation
cmake --install . --prefix=../install/x64_Release --config Release
CD ..

:: Exit to original path
CD ..
ECHO Windows CMake Build Done
