@ECHO OFF
SET README_PATH=%CD%\README.md
IF EXIST %README_PATH% (
    REM DO NOTHING
) ELSE (
    ECHO Error: You must run this script at the root folder of this project!
    EXIT /b
)

:: Create essential folder
MKDIR bin
CD bin
MKDIR Win32
MKDIR x64
MKDIR install

:: Build for Win32
CD Win32
cmake -G "Visual Studio 16 2019" -A Win32 -DYYCC_BUILD_TESTBENCH=ON ../..
cmake --build . --config Debug
cmake --install . --prefix=../install --config Debug
cmake --build . --config Release
cmake --install . --prefix=../install --config Release
CD ..

:: Build for x64
CD x64
cmake -G "Visual Studio 16 2019" -A x64 -DYYCC_BUILD_TESTBENCH=ON ../..
cmake --build . --config Debug
cmake --install . --prefix=../install --config Debug
cmake --build . --config Release
cmake --install . --prefix=../install --config Release
CD ..

ECHO DONE
