# YYC Commonplace

YYC Commonplace, or YYCCommonplace (abbr. YYCC) is a static library specifically resolving my requirements in C++ and Windows scope.

## Usage

For more usage about this library, please build documentation of this project via Doxygen and read it.

And I also highly recommend that you read documentation first before writing with this library.

However, the documentation need CMake to build and you may don't know how to use CMake in this project. So as the alternative, you also can browse the raw Doxygen documentation file: `doc/src/intro.dox` for how to build this project (including documentation) first.

## Build

This project require at least CMake 3.23 to build. We suggest that you only use stable version (tagged commit). The latest commit may still work in progress and not stable.

See documentation for how to build this project.

> [!INFO]  
> When building with testbench, you may face link error with GoogleTest. This issue is caused by that the binary provided by your package manager is built in C++ 17 and its ABI is incompatible with C++ 23. The solution is that download GoogleTest source code and build it in C++ 23 on your own. See this [GitHub Issue](https://github.com/google/googletest/issues/4591) for more infomation.
> Oppositely, you don't need care about this issue if you just want to build YYCC self.

