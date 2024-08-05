@ECHO OFF
SET README_PATH=%CD%\README.md
IF EXIST %README_PATH% (
    REM DO NOTHING
) ELSE (
    ECHO Error: You must run this script at the root folder of this project!
    EXIT /b
)

:: Enter main binary directory
CD bin

:: Create MSVC binary directory
MKDIR msvc_install
CD msvc_install
:: Create direcotries tree
MKDIR bin
MKDIR include
MKDIR lib
MKDIR share
CD bin
MKDIR Win32
MKDIR x64
CD ..
CD lib
MKDIR Win32\Debug
MKDIR Win32\Release
MKDIR x64\Debug
MKDIR x64\Release
CD ..
:: Exit MSVC binary directory
CD ..

:: Copy result
:: Copy include from x64_Release build
XCOPY install\x64_Release\include msvc_install\include\ /E /Y
:: Copy document from x64_Release build
IF NOT "%1"=="NODOC" (
    XCOPY install\x64_Release\share msvc_install\share\ /E /Y
)
:: Copy binary testbench
COPY install\Win32_Release\bin\YYCCTestbench.exe msvc_install\bin\Win32\YYCCTestbench.exe /Y
COPY install\x64_Release\bin\YYCCTestbench.exe msvc_install\bin\x64\YYCCTestbench.exe /Y
:: Copy static library
COPY install\Win32_Debug\lib\YYCCommonplace.lib msvc_install\lib\Win32\Debug\YYCCommonplace.lib /Y
COPY install\Win32_Release\lib\YYCCommonplace.lib msvc_install\lib\Win32\Release\YYCCommonplace.lib /Y
COPY install\x64_Debug\lib\YYCCommonplace.lib msvc_install\lib\x64\Debug\YYCCommonplace.lib /Y
COPY install\x64_Release\lib\YYCCommonplace.lib msvc_install\lib\x64\Release\YYCCommonplace.lib /Y

:: Exit to original path
CD ..
ECHO Windows MSVC Build Done
