:: Navigate to project root directory
CD /d J:\YYCCommonplace
:: Create build directory and enter it
MKDIR bin
CD bin
MKDIR cpp20
CD cpp20

:: Create internal build directory
MKDIR Win32
MKDIR x64
MKDIR documentation
:: Create internal install directory
MKDIR install
CD install
MKDIR Win32_Debug
MKDIR Win32_Release
MKDIR x64_Debug
MKDIR x64_Release
CD ..
:: Create internal MSVC specific install directory
MKDIR msvc_install
CD msvc_install
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
CD ..

:: Build for Win32
CD Win32
cmake -G "Visual Studio 16 2019" -A Win32 -DCMAKE_CXX_STANDARD=20 -DYYCC_BUILD_TESTBENCH=ON ../..
cmake --build . --config Debug
cmake --install . --prefix=../install/Win32_Debug --config Debug
cmake --build . --config Release
cmake --install . --prefix=../install/Win32_Release --config Release
CD ..
:: Build for x64
CD x64
cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_CXX_STANDARD=20 -DYYCC_BUILD_TESTBENCH=ON ../..
cmake --build . --config Debug
cmake --install . --prefix=../install/x64_Debug --config Debug
cmake --build . --config Release
cmake --install . --prefix=../install/x64_Release --config Release
CD ..

:: Copy header files
XCOPY install\x64_Release\include msvc_install\include\ /E /Y
:: Copy binary files
COPY install\Win32_Release\bin\YYCCTestbench.exe msvc_install\bin\Win32\YYCCTestbench.exe /Y
COPY install\x64_Release\bin\YYCCTestbench.exe msvc_install\bin\x64\YYCCTestbench.exe /Y
:: Copy library files
COPY install\Win32_Debug\lib\YYCCommonplace.lib msvc_install\lib\Win32\Debug\YYCCommonplace.lib /Y
COPY install\Win32_Release\lib\YYCCommonplace.lib msvc_install\lib\Win32\Release\YYCCommonplace.lib /Y
COPY install\x64_Debug\lib\YYCCommonplace.lib msvc_install\lib\x64\Debug\YYCCommonplace.lib /Y
COPY install\x64_Release\lib\YYCCommonplace.lib msvc_install\lib\x64\Release\YYCCommonplace.lib /Y

:: Leave build directory and report
CD ..\..
ECHO Windows CMake Build Done
