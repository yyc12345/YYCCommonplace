# Compile Manual

## Choose Version

This manual is only suit for the version equal or newer than YYCC 2.0.
For old version, please checkout to corresponding tag and browse how to build them.

We suggest that you only use stable version (tagged commit).
The latest commit always present current works.
It means that it is not stable and work in progress.

## Requirements

* CMake 3.23 at least.
* The common compiler supporting C++ 23 (GCC / Clang / MSVC).
* Iconv (Optional on Windows. Required on other systems).
* [Google Test](https://github.com/google/googletest) (Required if you build test).
* [Google Benchmark](https://github.com/google/benchmark) (Required if you build benchmark).
* Doxygen (Required if you build documentation).

If you are just want to build this project to make something works, or build other project, rather than code with it,
you commonly do not need build test, benchmark and documentation.
So you actually do not need Google Test, Google Benchmark and Doxygen.

## Preparing

### Compiler

> [!WARNING]  
> You may face some issues when building on macOS with Apple Clang. That's not your fault.
> Clang and Apple Clang used libc++ library lacks some essential features used by this project.
> This is especially not good for Apple Clang because Apple Clang is usually behind Clang a bunch of versions.
> 
> For resolving this issue, I have written a series of patch header files for libcxx and you can find them in include directory.
> This project should be compiled on macOS but everything has exception.
> If you really have this issue, a possible solution is that use GCC and libstdc++ on macOS instead of default Clang and libc++.
> 
> Build issue may be resolved until libc++ finish these features: complete `std::from_chars` and `std::to_chars`,
> `std::stacktrace` and `std::views::enumerate`.

### Google Test

Google Test is required if you need to build test.
If you don't need this please skip this chapter.

We use Google Test v1.17.0.
It would be okey use other versions but I have not test on them.

> [!WARNING]  
> When building this project, you may face link error with Google Test, especially on Linux.
> This issue is caused by that the binary provided by your package manager is built in C++17 and its ABI is incompatible with C++23.
> See this [GitHub Issue](https://github.com/google/googletest/issues/4591) for more infomation.
> The solution is that download Google Test source code and build it in C++23 on your own.
> Following content tell you how to do this.

There are the steps instructing you how to compile Google Test manually.

1. Download Google Test source code with given version in GitHub Release page.
1. Extract it into a directory.
1. Enter this directory and create 2 subdirectory `build` and `install` for CMake build and install respectively.
1. Enter `build` directory and configure CMake with extra `-DCMAKE_CXX_STANDARD=23 -Dgtest_force_shared_crt=ON` parameters.
1. Use CMake to build Google Test
1. Use CMake to install Google Test into previous we created `install` directory.

### Google Benchmark

Google Benchmark is required if you need to build benchmark.
If you don't need this please skip this chapter.

We use Google Benchmark v1.9.4.
It would be okey use other versions but I have not test on them.

There are the steps instructing you how to compile Google Benchmark manually.

1. Download Google Benchmark source code with given version in GitHub Release page.
1. Extract it into a directory.
1. Enter this directory and create link named `googletest` to previous fetched Google Test root directory. This is instructed by official manual because Google Benchmark rely on Google Test. Link can be create by executing `mklink /D googletest <path-to-googletest-root-dir>` on Windows or `ln -s <path-to-googletest-root-dir> googletest` on POSIX-like OS.
1. Keep stay in this directory and create 2 subdirectory `build` and `install` for CMake build and install respectively.
1. Enter `build` directory and configure CMake with extra `-DCMAKE_CXX_STANDARD=23 -DBENCHMARK_ENABLE_TESTING=OFF` parameters.
1. Use CMake to build Google Benchmark
1. Use CMake to install Google Benchmark into previous we created `install` directory.

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
If you are the user of this project (just want this project to make something works, or build other projects), please choose "User Build".
If you are a developer (developer of this project, or use this project as dependency to develop your project), please choose "Developer Build".

### User Build

"User Build" is basically how GitHub Action build this project.

Under **the root directory** of this project, execute:

- `script/windows_build.bat` on Windows
- or `script/linux_build.sh` on Linux
- or `script/macos_build.sh` on macOS

The final built artifact is under `bin/install` directory.

### Developer Build

#### Configurable Variables

First, there is a list listing all variables you may configure during compiling.

* `YYCC_BUILD_TEST`: Set it to `ON` to build test. `OFF` in default.
It is useful for the developer of this project.
It also suit for the user who has runtime issues on their platforms to check whether this project works as expected.
If you are debugging this project to find bug, I suggest that you build this project under Debug mode and use this test project for debugging.
* `YYCC_BUILD_BENCHMARK`: Set it to `ON` to build benchmark. `OFF` in default.
It is useful for the developer of this project to checking the performace for those homemade functions.
It is highly suggested build this project with Release mode to have real benchmark result.
* `YYCC_BUILD_DOC`: Set it to `ON` to build documentation. `OFF` in default.
It may be useful for the developer who firstly use this project in their own projects.
Please note that generated documentation is different in different platforms.
* `YYCC_ENFORCE_ICONV`: Set it to `ON` to enable Iconv feature forcely. `OFF` in default.
The usage of this option has been introduced in previous "Iconv" chapter.
* `GTest_ROOT`: Set to the install path of Google Test
if you have enable `YYCC_BUILD_TEST` and want to use your personal built Google Test.
* `benchmark_ROOT`: Set to the install path of Google Benchmark
if you have enable `YYCC_BUILD_BENCHMARK` and want to use your personal built Google Benchmark.
* `Iconv_ROOT`: The assistant variable for finding Iconv which is exposed by CMake.
You usually do not need set it up.
* `CMAKE_CXX_STANDARD`: Set C++ standard version of project.
`23` in default and this version can not be lower than C++23.
You usually do not need change this.
* `CMAKE_POSITION_INDEPENDENT_CODE`: Set it to `True` to enable PIC.
This is essential for those project which use this project and produce dynamicing library as final artifact.

#### Configure CMake

When configure CMake, you may use different options on different platforms.
Following list may help you.

- On Windows:
    * `-A Win32` or `-A x64` to specify architecture.
- On Linux or other POSIX systems:
    * `-DCMAKE_BUILD_TYPE=Debug` or `-DCMAKE_BUILD_TYPE=Release` to specify build type.

Additionally, you can attach any variables introduced above with `-D` option during CMake configurations.

#### Build with CMake

After configuration, you can use `cmake --build .` to build project,
with additional options on different platforms.
Following list may help you.

- On Windows:
    * `--config Debug` or `--config Release` to specify build type.
- On Linux or other POSIX systems:
    * None

#### Install with CMake

After building, you can use `cmake --install . --prefix <path-to-prefix>`
to install project into given path, with additional options on different platforms.
Following list may help you.

- On Windows:
    * `--config Debug` or `--config Release` to specify build type.
- On Linux or other POSIX systems:
    * None
