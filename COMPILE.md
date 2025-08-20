# Compile Manual

## Choose Version

We suggest that you only use stable version (tagged commit).
The latest commit always present current works.
It means that it is not stable and work in progress.

## Requirements

* CMake 3.23 at least.
* The common compiler supporting C++ 23 (GCC / Clang / MSVC).
* Iconv (Optional on Windows. Required on other systems).
* [GoogleTest](https://github.com/google/googletest) (Required if you build testbench).
* Doxygen (Required if you build documentation).
* Python and Astral UV (Required if you use "User Build" method)

> [!WARNING]  
> You may face some issues when building on macOS with Clang. That's not your fault.
> Clang used libc++ library lacks some essential features used by this project.
> You may try other solutions for compiling this project on macOS or with Clang.

## Preparing

### GoogleTest

GoogleTest is required if you need to build testbench.
If you don't need this please skip this chapter.

We use GoogleTest v1.17.0.
It would be okey use other versions but I have not test on them.

> [!WARNING]  
> When building this project, you may face link error with GoogleTest, especially on Linux.
> This issue is caused by that the binary provided by your package manager is built in C++17 and its ABI is incompatible with C++23.
> See this [GitHub Issue](https://github.com/google/googletest/issues/4591) for more infomation.
> The solution is that download GoogleTest source code and build it in C++23 on your own.
> Following content tell you how to do this.

There are the steps instructing you how to compile GoogleTest manually.

1. Download GoogleTest source code with given version in GitHub Release page.
1. Extract it into a directory.
1. Enter this directory and create 2 subdirectory `build` and `install` for CMake build and install respectively.
1. Enter `build` directory and configure CMake with extra `-DCMAKE_CXX_STANDARD=23 -Dgtest_force_shared_crt=ON` parameters.
1. Use CMake to build GoogleTest
1. Use CMake to install GoogleTest into previous we created `install` directory.

### Iconv

Iconv is optional on Windows and disabled in default.
However, if you are building project on MSYS2 or MINGW platform in Windows, we suggest you enable Iconv feature for more functions.
Once you enable this feature, you must prepare installed Iconv which is no problem for MSYS2 environment via package manager.
You also can enable this feature under MSVC but you must make sure that you can compile Iconv under MSVC.
For how to enable this feature forcely, see following chapters for more infomations.

On other platforms, Iconv is enabled automatically and can not be disabled.
Because there is no other encoding convertion libraries that we can use (Windows has a builtin set of encoding convertion Win32 functions).

### Doxygen

Doxygen is required only if you need to build documentation.
If you don't need this please skip this chapter.

We use Doxygen 1.9.7.
It would be okey use other versions but I have not test on them.

YYCCommonplace use Doxygen as its documentation system.
So before compiling, you must make sure `doxygen` are presented in your environment.

## Build and Install

There are 2 different ways to build this project.
If you are the user of this project (just want this project to make something work), please choose "User Build".
If you are a developer (developer of this project, or use this project as dependency to develop your project), please choose "Developer Build".

### User Build

"User Build" is basically how GitHub Action build this project.

We use Python 3.11 and UV 0.7.17 to manage our build script generator.
It would be okey use other versions but I have not test on them.

TODO...

### Developer Build

TODO...

There is a list listing all variables you may configure during compiling.

* `YYCC_BUILD_TESTBENCH`: Set it to `ON` to build testbench. `OFF` in default.
It is useful for the developer of this project.
It also suit for the user who has runtime issues on their platforms to check whether this project works as expected.
* `YYCC_BUILD_DOC`: Set it to `ON` to build documentation. `OFF` in default.
It may be useful for the developer who firstly use this project in their own projects.
Please note that generated documentation is different in different platforms.
* `YYCC_ENFORCE_ICONV`: Set it to `ON` to enable Iconv feature forcely. `OFF` in default.
The usage of this option has been introduced in previous "Iconv" chapter.
* `GTest_ROOT`: TODO
* `Iconv_ROOT`: TODO
* `CMAKE_CXX_STANDARD`: Set C++ standard version of project.
`23` in default and this version can not be lower than C++23.
You usually do not need change this.
* `CMAKE_POSITION_INDEPENDENT_CODE`: Set it to `True` to enable PIC.
This is essential for those project which use this project and produce dynamicing library as final artifact.
