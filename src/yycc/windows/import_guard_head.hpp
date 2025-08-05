// It is by design that no pragma once or #if to prevent deplicated including.
// Because this header is the part of wrapper, not a real header.
// #pragma once

// YYC MARK:
// Since YYCC 2.0 version, we use CMake to handle Windows shitty macros,
// so we do not need declare WIN32_LEAN_AND_MEAN or NOMINMAX in there.
// But for keeping the pair of this guard, we still keep this header file,
// although it do nothing.
